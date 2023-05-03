//  Use the OpenWeather library: https://github.com/Bodmer/OpenWeather
//
//  The weather icons and fonts are in the sketch data folder, press Ctrl+K to view.
//
//  >>> IMPORTANT TO PREVENT CRASHES <<<
//  >>>>>>  Set SPIFFS to at least 1.5Mbytes before uploading files  <<<<<<
//
//  >>> DON'T FORGET THIS <<<
//    Alle WiFi gegevens aanpassen,
//    API key invoern,
//    board: LOLIN D1 R2 & mini, juiste compoort,
//    flashsize 4MB FS:2MB.... ,
//    ESP8266 sketch data upload uitgevoeren,
//    Upload the fonts and icons to SPIFFS using the "Tools"  "ESP32 Sketch Data Upload"
//    or "ESP8266 Sketch Data Upload" menu option in the IDE.
//    WHIT SCREEN: ->->-> Libraries\TFT_eSPI-master, ->->-> User_Setup_Select.h ->->-> activate:  #include <Setup4_ILI9341_8266.h>
//
//  To add this option follow instructions here for the ESP8266:
//  https://github.com/esp8266/arduino-esp8266fs-plugin
//  To add this option follow instructions here for the ESP32:
//  https://github.com/me-no-dev/arduino-esp32fs-plugin
//
//  Close the IDE and open again to see the new menu option.
//
// You can change the number of hours and days for the forecast in the
// "User_Setup.h" file inside the OpenWeather library folder.
// By default this is 6 hours (can be up to 48) and 5 days
// (can be up to 8 days = today plus 7 days). This sketch requires
// at least 5 days of forecast. Forecast hours can be set to 1 as
// the hourly forecast data is not used in this sketch.
//
// Setttings defined below

Settings settings = {
    '%',                //chkDigit
    "MARODEKWiFi",      //wifiSSID[25];
    "MAROWiFi19052004!",//wifiPass[25];   
    1                   //isDebug
};

wlanSSID wifiNetworks[] {
    {"PI4RAZ","PI4RAZ_Zoetermeer"},
    {"Loretz_Gast", "Lor_Steg_98"},
    {"MARODEKWiFi", "MAROWiFi19052004!"},
    {"FHanna", "Kleinsteg71"}
};

// jpg file to display at startup. Must be uploaded to flash memory once using IDE menu option
// "Tools"  -> "ESP32 Sketch Data Upload" or "ESP8266 Sketch Data Upload".
// https://github.com/esp8266/arduino-esp8266fs-plugin
const int PAGE_DELAY_SECS = 3;                // time in secs a page is shown

// Set the forecast longitude and latitude to at least 4 decimal places
WeatherStation weatherStation[] = { // Max 10
  {"Zoetermeer"       , "52.0520"   , "4.4747"      , "/splash/steg.jpg"},
  {"Sinderen"         , "51.9037113", "6.4613935"   , "/splash/steg.jpg"},
  {"Poet Laval"       , "44.528251" , "5.021722"    , "/splash/steg.jpg"},
  {"Steg"             , "47.107914" , "9.578158"    , "/splash/steg.jpg"},
  {"Cambridge Bay"    , "69.107082" , "-105.1217674", "/splash/steg.jpg"}, 
  {"Noord-pool"       , "90.0"      , "0.0"         , "/splash/steg.jpg"},
  {"Zuid-pool"        , "-90.0"     , "0.0"         , "/splash/steg.jpg"},
  {"Oymyakon"         ,"63.464138"  , "142.773727"  , "/splash/steg.jpg"}      // gemeten koudste plek op aarde
};

#define TIMEZONE euCET // See NTP_Time.h tab for other "Zone references", UK, usMT etc
#define MQTT_MESSAGES 
//#define HasLocalTemp
const char* mqtt_broker   = "mqtt.rjdekok.nl";
const char* mqtt_user     = "Robert";
const char* mqtt_pass     = "Mosq5495!";
const int mqtt_port       = 1883;
const String phoneNumber  = "0651847704";
const String apiKey       = "NoKey";
const String mqtt_subject = "rdkstation/";

// Update every 15 minutes, up to 1000 request per day are free (viz average of ~40 per hour)
const int UPDATE_INTERVAL_SECS = 15 * 60UL;   // 15 minutes -- was 15 * 60UL
int startPage = 0;                            // Page to start with by default.
int screenRotation = 0;                       // 0=0, 1=90, 2=180, 3=270

// Pins for the TFT interface are defined in the User_Config.h file inside the TFT_eSPI library

// For units use "metric" or "imperial"
const String units = "metric";

// Sign up for a key and read API configuration info here:
// https://openweathermap.org/, change x's to your API key
const String api_key = "aa74ca1fd733d92c34ac4c2a6c73ef99";

// For language codes see https://openweathermap.org/current#multi
const String language = "nl"; // Default language = en = English

// Short day of week abbreviations used in 4 day forecast (change to your language)
const String shortDOW [8] = {"???", "Zon", "Maa", "Din", "Woe", "Don", "Vrij", "Zat"};

// Change the labels to your language here:
const char sunStr[]        = "Zon";
const char cloudStr[]      = "Bewolking";
const char humidityStr[]   = "Vochtigheid";
const String moonPhase [13] = {"Nieuw", "Wassend", "Eerste kwrt", "Wassend", "Vol", "Afnemend", "Laatste kwrt", "Afnemend"};
// Wind direction (change to your language)
// const String winddir[8] = {"N", "NO", "O", "ZO", "Z", "ZW", "W", "NW" };
const String winddir[8] = {"N", "NO", "O", "ZO", "Z", "ZW", "W", "NW" };
// End of user settings
//////////////////////////////
