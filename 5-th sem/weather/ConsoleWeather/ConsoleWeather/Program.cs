using System;
using System.Xml;
using System.Net;
using System.Collections;
using System.Collections.Generic;

namespace ConsoleWeather
{
    class Program
    {
        static void Main(string[] args)
        {
            GetWeather service = new GetWeather();
            
            Console.WriteLine("Hello there!! Weather!!!");
            service.Update();
        }
    }
}



public class GetWeather
{
    // Start is called before the first frame update

    public float updateInterval;

    public string CityName;

    string API_KEY;

    string weburl;
    
    XmlDocument doc;
    string szTemp;
    double temp;

    string szSun;



    // Update is called once per frame
    public void Update()
    {
        CityName = "Omsk";
        API_KEY = "368e827be4b38db51ff960ca88b5c396";

        weburl = "http://api.openweathermap.org/data/2.5/weather?q=" + CityName + "&mode=xml&APPID=" + API_KEY;
        //weburl = "http://api.openweathermap.org/data/2.5/forecast?id=524901&APPID=368e827be4b38db51ff960ca88b5c396";

        doc = new XmlDocument ();
        
        var xml = new WebClient().DownloadString(new Uri (weburl));

        // doc.LoadXml (xml);
        // foreach(XmlNode node in doc.GetElementsByTagName("*"))
        // {
        //     Debug.Log(node.Name);
        // }
        szTemp = doc.DocumentElement.SelectSingleNode ("temperature").Attributes["value"].Value;
        szSun = doc.DocumentElement.SelectSingleNode("sun").Attributes["value"].Value;
        temp = double.Parse (szTemp) - 273.16;
        Console.Out.WriteLine("Temp. " + temp.ToString("N2") + " Celcius" + ", Sun: " + szSun);
    }
}
