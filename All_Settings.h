Settings settings = {
    '&',                                //chkDigit
    "YourSSID",                         //wifiSSID[25];
    "YourWiFiPass!",                    //wifiPass[25];   
    "your openWether API key here",     //openWeatherAPI;
    0,                                  //useMQTT                       
    "mqtt.xxxx.nl",                     //mqttBroker;
    "MQTTUser",                         //mqttUser; 
    "MQTTPass",                         //mqttPass;
    "weatherStation/",                  //mqttSubject;  
    1,                                  //mqttTXUnits;    
    1883,                               //mqttPort; 
    0,                                  //useWapp;
    "+31612345678",                       //wappPhone;
    "your WhatsAppAPI key here",        //wappAPI;
    900,                                //wappInterval;
    1,                                  //serialMessages;
    0,                                  //hasLocalTempSensor;
    0,                                  //formatSpiffs;
    900,                                //updateInterval;
    3,                                  //pageDelay
    0,                                  //actualWeatherStation
    "Sinderen",                         //City1
    "51.9037113",                       //Latitude1
    "6.4613935",                        //Longitude1
    "Zoetermeer",                       //City2
    "52.0520",                          //Latitude2
    "4.4747",                           //Longitude2
    "Steg",                             //City3
    "47.107914",                        //Latitude3
    "9.578158",                         //Longitude3
    "Cambridge Bay",                    //City4
    "69.107082",                        //Latitude4
    "-105.12177",                       //Longitude4
    1,                                  //isDebug
    0,                                  //reverseRotation
    304,                                //touchRotation calData[0]
    3493,                               //touchRotation calData[1]
    345,                                //touchRotation calData[2]
    3499,                               //touchRotation calData[3]
    4                                   //touchRotation calData[4]
};

wlanSSID wifiNetworks[] {
    {"PI4RAZ","PI4RAZ_Zoetermeer"},
    {"Loretz_Gast","Lor_Steg_98"},
    {"MARODEKWiFi", "MAROWiFi19052004!"}
};

const char *splashFile = "/splash/steg.jpg";

// Set the forecast longitude and latitude to at least 4 decimal places
WeatherStation weatherStation[6] = { // Max 10
  {"Noord-pool"       , "90.0"      , "0.0"         },
  {"Zuid-pool"        , "-90.0"     , "0.0"         }
};

const String shortDOW [8]   = {"???", "Zon", "Maa", "Din", "Woe", "Don", "Vrij", "Zat"};
const char sunStr[]         = "Zon";
const char cloudStr[]       = "Bewolking";
const char humidityStr[]    = "Vochtigheid";
const String moonPhase[8]  = {"Nieuw", "Wassend", "Eerste kwrt", "Wassend", "Vol", "Afnemend", "Laatste kwrt", "Afnemend"};
const String winddir[8]     = {"N", "NO", "O", "ZO", "Z", "ZW", "W", "NW" };