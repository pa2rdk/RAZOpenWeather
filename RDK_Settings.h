Settings settings = {
    '^',                                //chkDigit
    "MARODEKWiFi",                      //wifiSSID[25];
    "MAROWiFi19052004!",                //wifiPass[25];   
    1,                                  //useMQTT                       
    "mqtt.rjdekok.nl",                  //mqttBroker;
    "Robert",                           //mqttUser; 
    "Mosq5495!",                        //mqttPass;
    "rdkstation/",                      //mqttSubject;  
    1883,                               //mqttPort; 
    0,                                  //useWapp;
    "0651847704",                       //wappPhone;
    "aa74ca1fd733d92c34ac4c2a6c73ef99", //wappAPI;
    900,                                //wappInterval;
    1,                                  //serialMessages;
    0,                                  //hasLocalTempSensor;
    0,                                  //formatSpiffs;
    900,                                //updateInterval;
    3,                                  //pageDelay
    1,                                  //isDebug
    "Sinderen",                         //City1
    "51.9037113",                       //Latitude1
    "6.4613935",                        //Longitude1
    "Zoetermeer",                       //City2
    "52.0520",                          //Latitude2
    "4.4747"                            //Longitude2
};

wlanSSID wifiNetworks[] {
    {"PI4RAZ","PI4RAZ_Zoetermeer"},
    {"Loretz_Gast", "Lor_Steg_98"}
};

const char *splashFile = "/splash/steg.jpg";

// Set the forecast longitude and latitude to at least 4 decimal places
WeatherStation weatherStation[] = { // Max 10
  // {"Zoetermeer"       , "52.0520"   , "4.4747"      },
  // {"Poet Laval"       , "44.528251" , "5.021722"    },
  // {"Steg"             , "47.107914" , "9.578158"    },
  // {"Cambridge Bay"    , "69.107082" , "-105.1217674"}, 
  {"Noord-pool"       , "90.0"      , "0.0"         },
  {"Zuid-pool"        , "-90.0"     , "0.0"         },
  // {"Oymyakon"         ,"63.464138"  , "142.773727"  }      // gemeten koudste plek op aarde
};

int screenRotation          = 0; // 0=0, 1=90, 2=180, 3=270
const String shortDOW [8]   = {"???", "Zon", "Maa", "Din", "Woe", "Don", "Vrij", "Zat"};
const char sunStr[]         = "Zon";
const char cloudStr[]       = "Bewolking";
const char humidityStr[]    = "Vochtigheid";
const String moonPhase [13] = {"Nieuw", "Wassend", "Eerste kwrt", "Wassend", "Vol", "Afnemend", "Laatste kwrt", "Afnemend"};
const String winddir[8]     = {"N", "NO", "O", "ZO", "Z", "ZW", "W", "NW" };