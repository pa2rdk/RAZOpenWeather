// *************************************************************************************
//  V4.1    04-05-25 Added conditions to MQTT
//  V3.2    17-10-24 MQTT fixes for symbols
//  V3.1    31-05-24 Auto refresh bug 
//  V2.8    05-05-24 First OTA Update
//  V2.5    04-05-24 OTA & RDKOTA Library - Releaseversion
//  V2.3    19-03-24 Version number on screen
//  V2.2.2  12-03-24 OpenWeather library compatible with OneCall2.5 and OneCall3.0
//                   Library included in github
//                   Some small warnings removed
//                   Blue moon removed            
//  V2.2.1  26-12-23 Calibrate touch en scherm draaibaar.
//  V2.2.0  26-11-23 Setting wel/niet meesturen symbool in MQTT.
//                   Refresh pages bij opstarten.
//                   MQTT connect 3x controleren               
//  V2.1.1  16-11-23 Geselecteerde locatie werd niet opgeslagen, display aangepast van 22 naar 14 ivm nieuwe printen
//  V2.1.0  11-06-23 Settings and reboot prohibited from external
//  V2.0.9  08-06-23  - Velden voor lat en kon verlengd van 15 naar 25 chars
//                    - Reset ingebouwd als bij het opstarten op het scherm wordt gedrukt.
//                    - Een time-out op de MQTT connectie. Als die na 5 seconden faalt, zet ik de MQTT connectie uit.
//                    - Foutjes op de settings webpage bij city3, 4 en 5. 
//  V2.0.8  06-06-23 Settings page fits on phone
//                   Auto refresh after selecting other location in browser
//                   Forecast from today          
//  V2.0.7  05-06-23 Show IP address
//  V2.0.6  21-05-23 Vertical layout of webpage repaired for phone
//  V2.0.5  18-05-23 Eliminated the usage of external images, use them from SPIFFS
//  V2.0.4  11-05-23 Bug with useWapp and moontext
//  V2.0.3  11-05-23 Int. tempsensor automatic enabled and placed on website.
//  V2.0.2  10-05-23 Tel. nr. longer and adjustments from Henny.
//  V2.0.1  09-05-23 Select location online
//  V2.0.0  01-05-23 Websettings
//  V1.8.6  13-01-23 Webpage added
//  V1.8.5  05-12-22 Local temperature added
//  V1.8.4  01-12-22 Possibility to send weather report via Whatsapp added.
//  V1.8.3  15-11-22 Add multiple splashscreen en variabele pageDelay welke bepaalt hoelang een sub scherm getoond wordt
//  V1.8.2b 15-11-22 Override en gebruik van de messagebox voorde locationlist,
//  V1.8.2a 15-11-22 PA2HGJ changes
//                  line 779: add messageBox() function
//                  line 225: changed tft.drawString to messageBox
//                  line 264: log connected WiFi network
//                  line 270: show connected SSID in messageBox
//                  line 288: changed tft.drawString to messageBox
//                  line 440: use AXTLS on ESP8266 BearSSL on ESP32
//                  line 622: changed function calcWindDirection() to calcWindAngle()
//                  line 602: use calcWindAngle() to get correct icon name
//                  line 612: Display winddir from winddir[] array in AllSettings.h
//                  line 887 and 916 use calcWindAngle() for correct logging and MQTT MQTT_MESSAGES
//                  line 1021: changed handlePage1() function
//                                - add waiting message
//                                - first download data then draw screen
//
//  V1.8.2 07-11-22 Belgium MUF table (page2 in 2 versions)
//  V1.8.1 03-11-22 More renovation, WiFi reset
//  V1.8 27-10-2022 Code renovation by PA2RDK
//  V1.7 18-04-2022 Muf informatie toegevoegd
//  V1.6            Automatic login strongest available WiFi. Automatic login last used location.
//  V1.5            Regel378 en 500
//  V1.4 22-08-2020 Location-list added
//                  Added personal startup screen in line 226. Upload whatever picture in portrait format to Picasa,
//                  select picture, export in 320 format to datamap "TFT eSPI Open weather",execute ESP8266 Data Upload,
//                  update line 226 for image stored in Data map
//                  Line 224: corrected location splah screen
//                  deleted clear instructions line 228- 246
//                  Changed line 264, draw string into "Get weather data...."
//  V1.3 22-08-2020 Change windspeed from m/s to Baufort.
//  V1.2 21-07-2020 All_Settings.h moonPhase[8] changed to moonPhase[13]
//  V1.1 07-07-2020 Led display swithed on for ESP8266 E12
//
//  In this sketch we use the sketch of Peter DD6USB to obtain and display the propagation data.
//  To better integrate it into Bodmer's weather station sketch, changes have been made to both original sketches by Robert PA2RDK.
//  Thank you all very much for the excellent work.
//
//  Example from OpenWeather library: https://github.com/Bodmer/OpenWeather
//  Adapted by Bodmer to use the TFT_eSPI library:  https://github.com/Bodmer/TFT_eSPI
// *************************************************************************************

#define AA_FONT_SMALL "fonts/NotoSansBold15"  // 15 point sans serif bold
#define AA_FONT_LARGE "fonts/NotoSansBold36"  // 36 point sans serif bold
#define B_DD6USB      0x0006                        // 0,   0,   4  my preferred background color !!!
#define HAMQSL_HOST   "hamqsl.com"                  // source page in www
#define IONIAP_HOST   "www.ionosonde.iap-kborn.de"  // another source page in www
#define IONBE_HOST    "ionosphere.meteo.be"         // another source page in www
#define DEG2RAD       0.0174532925                  // Degrees to Radians conversion factor
#define INCANLGE      2                             // Minimum segment subtended angle and plotting angle increment (in degrees)
#define offsetEEPROM  32
#define EEPROM_SIZE   4096
#define TIMEZONE      euCET

#define OTAHOST      "https://www.rjdekok.nl/Updates/RAZOpenWeather"
#define VERSION       "v4.1"
//#define isCYD         //CYD Display (Cheap Yellow Display)
/***************************************************************************************
Change this in User_Setup_Select.h in the TFT_eSPI library 
if no CYD: #include <Setup1_ILI9341.h>  // Setup file configured for my ILI9341  2.8 inch
if CYD:    #include <Setup1_ILI9341CYDRAZ.h>  // Setup file configured for my ILI9341  2.8 inch on CYD
***************************************************************************************/

/***************************************************************************************
**                          Load the libraries and settings
***************************************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>           // https://github.com/Bodmer/TFT_eSPI
#include <OneWire.h>            // Local temp.
#include <DallasTemperature.h>  // Local temp.
#ifdef isCYD
  #include <XPT2046_Touchscreen.h>
#endif

// Additional functions
#include "GfxUi.h"          // Attached to this sketch
#include "SPIFFS.h"         // Attached to this sketch

// Multi Wifi added by PA3HK
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <WifiMulti.h>
#include <HTTPClient.h>  // toegevoegd
WiFiMulti wifiMulti;

#include <EEPROM.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <UrlEncode.h>
//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <RDKOTA.h>

// Json streaming parser:  (do not use IDE library manager version)
#include <JSON_Decoder.h>             // https://github.com/Bodmer/JSON_Decoder
#include <OpenWeather.h>              // Latest here: https://github.com/Bodmer/OpenWeather
#include "NTP_Time.h"                 // Attached to this sketch

/***************************************************************************************
**                          Locations
***************************************************************************************/
typedef struct {  // Location data
  uint16_t Xlocos;          // Xoffset
  uint16_t Xlocsr;          // X size rectang
  uint16_t Xlocnr;          // X next rectang
  uint16_t Ylocos;          // Yoffset
  uint16_t Ylocsr;          // Y size rectang
  uint16_t Ylocnr;          // Y next rectang
} WeatherLocation;

uint16_t Xfloc = 5;
uint16_t Yfloc = 10;

WeatherLocation weatherLocation[] = {
  {Xfloc, 230,  0, Yfloc, 30 ,  0},
  {Xfloc, 230,  0, Yfloc, 30 , 30},
  {Xfloc, 230,  0, Yfloc, 30 , 60},
  {Xfloc, 230,  0, Yfloc, 30 , 90},
  {Xfloc, 230,  0, Yfloc, 30 , 120},
  {Xfloc, 230,  0, Yfloc, 30 , 150},
  {Xfloc, 230,  0, Yfloc, 30 , 180},
  {Xfloc, 230,  0, Yfloc, 30 , 210},
  {Xfloc, 230,  0, Yfloc, 30 , 240},
  {Xfloc, 230,  0, Yfloc, 30 , 270} //10 Max.
};

/***************************************************************************************
**                          Define the globals and class instances
***************************************************************************************/
int8_t actualPage;                    // which page I am ?
uint16_t maxWebBytes = 1700;          // max. bytes to read from content
String solarBuf;                      // to hold the compressed content
String contentStrings[10];            // holds the IAP data
String tot;                           // Total string for WHATSAPP
long lastRefresh = millis();          // Last refresh in millis()
bool forceRefresh = true;             // Refresh after touch or internet access
long lastWHATSAPPRefresh = -1;        // Last WHATSAPPrefresh in millis()
long lastLocTempRefresh  = -1;        // last LocTempRefresh in millis;
int EEPROM_ADDRESS = 0;               // EEProm address
int maxPage = 5;                      // Nr. of available pages
String lastIAP = "";
long startTime = millis();
bool apMode = false;
bool printConfig = false;
int MQTTFailCounter = 0;
bool doTouch = false;

typedef struct {
  byte chkDigit;
  char wifiSSID[25];
  char wifiPass[25];
  char openWeatherAPI[35];
  bool useMQTT;
  char mqttBroker[25];
  char mqttUser[25];
  char mqttPass[25];
  char mqttSubject[25];
  bool mqttTXUnits;
  int mqttPort;
  bool useWapp;
  char wappPhone[15];
  char wappAPI[35];
  int wappInterval;
  bool serialMessages;
  bool hasLocalTempSensor;
  bool formatSpiffs;
  int updateInterval;
  int pageDelay;
  int actualWeatherStation;
  char city1[25];
  char latitude1[25];
  char longitude1[25];
  char city2[25];
  char latitude2[25];
  char longitude2[25];  
  char city3[25];
  char latitude3[25];
  char longitude3[25];  
  char city4[25];
  char latitude4[25];
  char longitude4[25];      
  bool isDebug;
  bool reverseRotation;
  uint16_t calData0;
  uint16_t calData1;
  uint16_t calData2;
  uint16_t calData3;
  uint16_t calData4;  
} Settings;

typedef struct {  // Location name data
  const char *name;
  String latitude;
  String longitude;
} WeatherStation;

typedef struct {  // WiFi Access
  const char *SSID;
  const char *PASSWORD;
} wlanSSID;

// check All_Settings.h for adapting to your needs
//#include "RDK_Settings.h"
#include "All_Settings.h"

const int nrOffLocations = (sizeof weatherStation / sizeof (WeatherStation)) - 1;

TFT_eSPI tft = TFT_eSPI();            // Invoke custom library

#ifdef isCYD
  #define XPT2046_IRQ 36   // T_IRQ
  #define XPT2046_MOSI 32  // T_DIN
  #define XPT2046_MISO 39  // T_OUT
  #define XPT2046_CLK 25   // T_CLK
  #define XPT2046_CS 33    // T_CS

  SPIClass touchscreenSPI = SPIClass(VSPI);
  XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
#endif

GfxUi ui = GfxUi(&tft);               // Jpeg and bmpDraw functions TODO: pull outside of a class

WiFiClientSecure httpsNet;
WiFiClient httpNet;
RDKOTA rdkOTA(OTAHOST);

#include <MQTT.h>
WiFiClient mqttNet;
MQTTClient client;

OW_Weather ow;                        // Weather forecast library instance
OW_current *current;                  // Pointer to structs that temporarily holds weather data
OW_hourly  *hourly;                   // Not used
OW_daily   *daily;

#define ONE_WIRE_BUS_PIN 13

//Swith LED at display
#ifdef isCYD
  #define Display_Led    21 
  #define displayon      1
#else
  #define Display_Led    14
  #define displayon      0
#endif

// #define Display_Led    22 //Oude print zonder connector en transistor voor LED
// #define displayon      1

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

DeviceAddress Probe01 = { 0x28, 0xFF, 0x00, 0x91, 0x6B, 0x18, 0x01, 0x86 }; // Temp. sensor
AsyncWebServer server(80);

#include "webpages.h"

/***************************************************************************************
**                          Setup
***************************************************************************************/
void setup() {
  Serial.begin(115200);
  pinMode(Display_Led, OUTPUT);
  digitalWrite(Display_Led, displayon);
  EEPROM.begin(EEPROM_SIZE);
  #ifdef isCYD
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(2);
  #endif  
  tft.begin();

  uint16_t touchX = 0, touchY = 0;
  bool pressed=false;
  #ifdef isCYD
    pressed =touchscreen.tirqTouched() && touchscreen.touched();
    if (pressed){
      TS_Point p = touchscreen.getPoint();
      touchX = map(p.x, 170, 3850, 1,240);
      touchY = map(p.y, 310, 3900, 1,320);
      printTouchToSerial(touchX, touchY, p.z);
    }
  #else
    pressed = tft.getTouch(&touchX, &touchY);
  #endif  
  if (pressed || !LoadConfig()){
    if (settings.isDebug) Serial.println(F("Writing defaults"));
    messageBox("Reset to default", TFT_WHITE, TFT_NAVY);
    SaveConfig();
    delay(2000);
  }
  LoadConfig();
  tft.setRotation(settings.reverseRotation?2:0);
  uint16_t calData[5] = {settings.calData0, settings.calData1, settings.calData2, settings.calData3, settings.calData4};
  tft.setTouch(calData);

  LoadWeatherLocations();
  settings.hasLocalTempSensor = (GetLocalTemp()>-50);
  Serial.printf("Local temp enabled:%s",settings.hasLocalTempSensor?"yes":"no");

  unsigned long timeout = millis();
  bool timedOut = false;

  locationList(); // Print locations
  
  bool showLocList = true;
  while (showLocList) {
    bool pressed = false;
    #ifdef isCYD
      pressed =touchscreen.tirqTouched() && touchscreen.touched();
      if (pressed){
        TS_Point p = touchscreen.getPoint();
        touchX = map(p.x, 170, 3850, 1,240);
        touchY = map(p.y, 310, 3900, 1,320);
        printTouchToSerial(touchX, touchY, p.z);
      }
    #else
      pressed = tft.getTouch(&touchX, &touchY);
    #endif  
    timedOut = (millis() - timeout) > 10000;
    if (pressed or timedOut) {
      showLocList = useLocation(timedOut, touchX, touchY);
    }
  }

  tft.fillScreen(TFT_BLACK);
  SPIFFS.begin();
  //listFiles();

  // Enable if you want to erase SPIFFS, this takes some time!
  // then disable and reload sketch to avoid reformatting on every boot!
  if (settings.formatSpiffs){
    tft.setTextDatum(BC_DATUM); // Bottom Centre datum
    tft.drawString("Formatting SPIFFS, so wait!", 120, 195); SPIFFS.format();
  }

  if (SPIFFS.exists(splashFile)) {
    ui.drawJpeg(splashFile,   0, 0);
    delay(2000);
  }
  messageBox(VERSION, TFT_WHITE, TFT_NAVY, 5, 215, 230, 24);
  delay(1000);

  bool font_missing = false;
  if (SPIFFS.exists("/fonts/NotoSansBold15.vlw")    == false) font_missing = true;
  if (SPIFFS.exists("/fonts/NotoSansBold36.vlw")    == false) font_missing = true;

  if (font_missing) {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    //while(1) yield();
  }

  // add Wi-Fi networks from All_Settings.h
  int maxNetworks = (sizeof(wifiNetworks) / sizeof(wlanSSID));

  for (int i = 0; i < maxNetworks; i++ )
    wifiMulti.addAP(wifiNetworks[i].SSID, wifiNetworks[i].PASSWORD);

  wifiMulti.addAP(settings.wifiSSID,settings.wifiPass);  

  messageBox("Verbinden met WiFi", TFT_WHITE, TFT_NAVY);
  if (Connect2WiFi()){
    Serial.print("Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    if (rdkOTA.checkForUpdate(VERSION)){
      if (questionBox("Installeer update", TFT_WHITE, TFT_NAVY, 5, 240, 230, 48)){
        messageBox("Installing update", TFT_YELLOW, TFT_NAVY, 5, 240, 230, 48);
        rdkOTA.installUpdate();
      } 
    }

    String SSID = WiFi.SSID();
    char ssid[SSID.length() + 1];
    SSID.toCharArray(ssid, SSID.length() + 1);

    char ipNo[16];
    sprintf(ipNo, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);

    messageBox(ssid, TFT_GREEN, TFT_NAVY);
    messageBox(ipNo, TFT_GREEN, TFT_NAVY, 5, 265, 230, 24);
  } else {
    messageBox("Connect to RAZWeather", TFT_GREEN, TFT_NAVY);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("RAZWeather", NULL);
    apMode = true;
  }
  delay(3000);  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (apMode)
      request->send_P(200, "text/html", settings_html, processor);
    else 
      request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/settings", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->client()->remoteIP()[0] == 192 || request->client()->remoteIP()[0] == 10 || request->client()->remoteIP()[0] == 172)
      request->send_P(200, "text/html", settings_html, processor);
    else
      request->send_P(200, "text/html", warning_html, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/css", css_html);
  });

   server.on("/plaatje", HTTP_GET, [](AsyncWebServerRequest *request){
    //"/43b4.png"
    if (request->hasParam("image")) request->getParam("image")->value();
    request->send(SPIFFS, request->getParam("image")->value(), "image/png");
  });

  server.on("/reboot", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->client()->remoteIP()[0] == 192 || request->client()->remoteIP()[0] == 10 || request->client()->remoteIP()[0] == 172){
      request->send(200, "text/plain", "Rebooting");
      ESP.restart();
    }
    else
      request->send_P(200, "text/html", warning_html, processor);
  });

  server.on("/calibrate", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->client()->remoteIP()[0] == 192 || request->client()->remoteIP()[0] == 10 || request->client()->remoteIP()[0] == 172){
      request->send_P(200, "text/html", index_html, processor);
      doTouch = true;
    }
    else
      request->send_P(200, "text/html", warning_html, processor);
  });

  server.on("/store", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->client()->remoteIP()[0] == 192 || request->client()->remoteIP()[0] == 10 || request->client()->remoteIP()[0] == 172){
      SaveSettings(request);
      SaveConfig();
      LoadWeatherLocations();
      printConfig=true;
      request->send_P(200, "text/html", index_html, processor);
    }
    else
      request->send_P(200, "text/html", warning_html, processor);
  }); 

  server.on("/golocation", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("location")) settings.actualWeatherStation = request->getParam("location")->value().toInt();
    actualPage = 0;
    SaveConfig();
    printConfig=true;
    request->send_P(200, "text/html", refresh_html, processor);
  });

  server.begin();
  Serial.println("HTTP server started");

  if (settings.useMQTT){
    Serial.println("Start MQTT");
    client.begin(settings.mqttBroker, settings.mqttPort, mqttNet);
    Serial.println("Started MQTT");
  }

  messageBox("Wacht op gegevens", TFT_WHITE, TFT_NAVY);

  //Fetch the time
  udp.begin(localPort);
  syncTime();

  //tft.unloadFont();
  ow.partialDataSet(true); // Collect a subset of the data available
  httpsNet.setInsecure();
  actualPage = 0;

  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  sensors.setResolution(Probe01, 10);

    // Create the structures that hold the retrieved weather
  current = new OW_current;
  daily =   new OW_daily;
  hourly =  new OW_hourly;

}

void LoadWeatherLocations(){
  WeatherStation weatherLocation;
  weatherLocation.name = settings.city1;
  weatherLocation.latitude = settings.latitude1;
  weatherLocation.longitude = settings.longitude1;
  weatherStation[2] = weatherLocation;
  weatherLocation.name = settings.city2;
  weatherLocation.latitude = settings.latitude2;
  weatherLocation.longitude = settings.longitude2;
  weatherStation[3] = weatherLocation;
  weatherLocation.name = settings.city3;
  weatherLocation.latitude = settings.latitude3;
  weatherLocation.longitude = settings.longitude3;
  weatherStation[4] = weatherLocation;
  weatherLocation.name = settings.city4;
  weatherLocation.latitude = settings.latitude4;
  weatherLocation.longitude = settings.longitude4;
  weatherStation[5] = weatherLocation;
}

bool Connect2WiFi(){
  startTime = millis();
  if (settings.isDebug) Serial.print("Connect to Multi WiFi");
  while (wifiMulti.run() != WL_CONNECTED && millis()-startTime<30000){
    // esp_task_wdt_reset();
    delay(1000);
    if (settings.isDebug) Serial.print(".");
  }
  if (settings.isDebug) Serial.println();
  return (WiFi.status() == WL_CONNECTED);
}

/***************************************************************************************
**                          Loop
***************************************************************************************/
void loop() {
  if (doTouch){
    doTouch = false;
    TouchCalibrate();
    forceRefresh = true;
  }

  uint16_t touchX = 0, touchY = 0;
  bool pressed = false;
  #ifdef isCYD
    pressed =touchscreen.tirqTouched() && touchscreen.touched();
    if (pressed){
      TS_Point p = touchscreen.getPoint();
      touchX = map(p.x, 170, 3850, 1,240);
      touchY = map(p.y, 310, 3900, 1,320);
      printTouchToSerial(touchX, touchY, p.z);
    }
  #else
    pressed = tft.getTouch(&touchX, &touchY);
  #endif 
  if (pressed)  {
    delay(200);
    if ((touchX > 180) and (touchY > 270)) {
      #ifdef isCYD
        messageBox("Herstarten...", TFT_WHITE, TFT_NAVY);
        delay(5000);
        ESP.restart();
      #else
        ESP.restart();
      #endif
    }
    if ((touchX <  90) and (touchY > 270)) {
      actualPage == maxPage ? actualPage = 0 : actualPage++;
      forceRefresh = true;
    }
    pressed = false;
  }

  if (printConfig){
    PrintConfig();
    forceRefresh = true;
    printConfig=false;
  }

  if (forceRefresh || ((millis() - lastRefresh > 1000UL * settings.updateInterval) && (millis() - lastRefresh > 1000UL * 120))) {
    lastRefresh = millis();
    Serial.println("Refresh page");
    if (!forceRefresh) {
      if (actualPage<maxPage){
        for (int x = actualPage + 1; x <= maxPage; x++) {
          handlePages(x);
          delay(settings.pageDelay * 1000);
        }
      }
      for (int x = 0; x <= actualPage; x++) {
        handlePages(x);
        delay(settings.pageDelay * 1000);
      }
    } else {
      forceRefresh = false;
      handlePages(actualPage);
    }
    lastRefresh = millis();
  }

  if (minute() != lastMinute) {
    Serial.println("Refresh minute");
    syncTime();
    if (actualPage == 0) drawTime();
    lastMinute = minute();
  }

  if (settings.useWapp){
    if (lastWHATSAPPRefresh == -1 || (millis() - lastWHATSAPPRefresh > 1000UL * settings.wappInterval)) {
      sendMessagetoWHATSAPP(tot);
      lastWHATSAPPRefresh = millis();
    }
  }
}
/***************************************************************************************/
void handlePages(int pageNr) {
  Serial.print("Handle page:");
  Serial.println(pageNr);

  //if (pageNr == 0) WiFi.mode(WIFI_AP_STA);
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  if (httpsNet.connected()) httpsNet.stop();
  if (httpNet.connected()) httpNet.stop();
  switch (pageNr)         // good for handling more pages (maybe in future)
  {
    case 0 :
      {
        handlePage0();  // Weather
        break;
      }
    case 1 :
      {
        handlePage1();  // Band usage
        break;
      }
    case 2 :
      {
        handlePage2(0);  // Muf DE
        break;
      }
    case 3 :
      {
        handlePage2(1);  // Muf BE
        break;
      }
    case 4 :
      {
        handlePage3();  // Overview 1
        break;
      }
    case 5 :
      {
        handlePage4();  // Overview 2
        break;
      }
  }
}

/***************************************************************************************
**                          Fetch the weather data  and update screen
**                          Update the Internet based information and update screen
***************************************************************************************/
void handlePage0() {
  tft.fillScreen(TFT_BLACK);
  updateWeather();
  syncTime();
  drawTime();
  lastMinute = minute();
}

/***************************************************************************************/
void updateWeather() {
  tft.loadFont(AA_FONT_SMALL);
  drawProgress(50, "Updating...");

  Serial.printf("Actual weather %d = from lat:%s and lon:%s\r\n",settings.actualWeatherStation, weatherStation[settings.actualWeatherStation].latitude, weatherStation[settings.actualWeatherStation].longitude);
  bool parsed = ow.getForecast(current, hourly, daily, settings.openWeatherAPI, weatherStation[settings.actualWeatherStation].latitude, weatherStation[settings.actualWeatherStation].longitude, "metric", "nl");
  tft.fillScreen(TFT_BLACK);
  printWeather(); // For debug, turn on output with #define SERIAL_MESSAGES

  if (lastRefresh == -1) {
    drawProgress(100, "Done...");
    delay(2000);
    tft.fillScreen(TFT_BLACK);
  }

  if (parsed) {
    drawCurrentWeather();
    drawForecast();
    drawAstronomy();

    tft.unloadFont();

    // Update the temperature here so we don't need to keep
    // loading and unloading font which takes time
    tft.loadFont(AA_FONT_SMALL);
    tft.loadFont(AA_FONT_LARGE);
    tft.setTextDatum(TR_DATUM);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    // Font ASCII code 0xB0 is a degree symbol, but o used instead in small font
    tft.setTextPadding(tft.textWidth(" -88")); // Max width of values

    String weatherText = "";
    weatherText = String(current->temp, 0);  // Make it integer temperature
    tft.drawString(weatherText, 215, 95); //  + "°" symbol is big... use o in small font
  } else {
    Serial.println("Failed to get weather");
  }

  if (settings.hasLocalTempSensor) PrintLocalTemp();
  tft.unloadFont();
}

/***************************************************************************************
**                          Update progress bar
***************************************************************************************/
void drawProgress(uint8_t percentage, String text) {
  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(240);
  tft.drawString(text, 120, 260);

  ui.drawProgressBar(10, 269, 240 - 20, 15, percentage, TFT_WHITE, TFT_BLUE);

  tft.setTextPadding(0);
}

/***************************************************************************************
**                          Draw the clock digits
***************************************************************************************/
void drawTime() {
  tft.loadFont(AA_FONT_LARGE);

  // Convert UTC to local time, returns zone code in tz1_Code, e.g "GMT"
  time_t local_time = TIMEZONE.toLocal(now(), &tz1_Code);

  String timeNow = "";

  if (hour(local_time) < 10) timeNow += "0";
  timeNow += hour(local_time);
  timeNow += ":";
  if (minute(local_time) < 10) timeNow += "0";
  timeNow += minute(local_time);

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" 44:44 "));  // String width + margin
  tft.drawString(timeNow, 120, 53);

  drawSeparator(51);

  tft.setTextPadding(0);

  tft.unloadFont();
}

/***************************************************************************************
**                          Draw the current weather
***************************************************************************************/
void drawCurrentWeather() {
  String date = "Updated: " + strDate(current->dt);

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(240);
  tft.drawString(weatherStation[settings.actualWeatherStation].name, 120, 16);
  String weatherIcon = "";

  String currentSummary = current->main;
  currentSummary.toLowerCase();

  weatherIcon = getMeteoconIcon(current->id, true);

  ui.drawBmp("/icon/" + weatherIcon + ".bmp", 0, 53);
  String weatherText = current->main;

  tft.setTextDatum(BR_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);

  int splitPoint = 0;
  int xpos = 235;
  splitPoint =  splitIndex(weatherText);

  tft.setTextPadding(xpos - 100);  // xpos - icon width
  if (splitPoint) tft.drawString(weatherText.substring(0, splitPoint), xpos, 69);
  else tft.drawString(" ", xpos, 69);
  tft.drawString(weatherText.substring(splitPoint), xpos, 86);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(0);
  tft.drawString("oC", 237, 95);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  weatherText = windspeedconv(current->wind_speed);
  weatherText += " Bft";

  tft.setTextDatum(TC_DATUM);
  tft.setTextPadding(tft.textWidth("88 Bft")); // Max string length?
  tft.drawString(weatherText, 124, 138);

  weatherText = String(current->pressure, 0);
  weatherText += " hPa";

  tft.setTextDatum(TR_DATUM);
  tft.setTextPadding(tft.textWidth(" 8888hPa")); // Max string length?
  tft.drawString(weatherText, 230, 138);

  int windAngle = calcWindAngle(current->wind_deg);
  String wind[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW" };
  ui.drawBmp("/wind/" + wind[windAngle] + ".bmp", 101, 86);
  drawSeparator(153);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.setTextPadding(tft.textWidth("88")); // Max string length?
  tft.drawString(winddir[windAngle], 126, 69);

  tft.setTextDatum(TL_DATUM); // Reset datum to normal
  tft.setTextPadding(0);      // Reset padding width to none
}

/***************************************************************************************
**                          Calculate degrees to winddirection
***************************************************************************************/
int calcWindAngle(uint16_t windDeg) {
  int windAngle = (windDeg + 22.5) / 45;
  if (windAngle > 7) windAngle = 0;
  return windAngle;
}

/***************************************************************************************
**                          Draw the 4 forecast columns
***************************************************************************************/
// draws the three forecast columns
void drawForecast() {
  int8_t dayIndex = 0;

  drawForecastDetail(  8, 171, dayIndex++);
  drawForecastDetail( 66, 171, dayIndex++); // was 95
  drawForecastDetail(124, 171, dayIndex++); // was 180
  drawForecastDetail(182, 171, dayIndex  ); // was 180
  drawSeparator(171 + 69);
}

/***************************************************************************************
**                          Draw 1 forecast column at x, y
***************************************************************************************/
// helper for the forecast columns
void drawForecastDetail(uint16_t x1, uint16_t y1, uint8_t dayIndex) {

  if (dayIndex >= MAX_DAYS) return;

  String day  = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[dayIndex], &tz1_Code))];
  day.toUpperCase();

  tft.setTextDatum(BC_DATUM);

  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("WWW"));
  tft.drawString(day, x1 + 25, y1);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("-88   -88"));
  String highTemp = String(daily->temp_max[dayIndex], 0);
  String lowTemp  = String(daily->temp_min[dayIndex], 0);
  tft.drawString(highTemp + " " + lowTemp, x1 + 25, y1 + 17);

  String weatherIcon = getMeteoconIcon(daily->id[dayIndex], false);
  ui.drawBmp("/icon50/" + weatherIcon + ".bmp", x1, y1 + 18);
  tft.setTextPadding(0); // Reset padding width to none
}

/***************************************************************************************
**                          Draw Sun rise/set, Moon, cloud cover and humidity
***************************************************************************************/
void drawAstronomy() {

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" Last qtr "));

  time_t local_time = TIMEZONE.toLocal(current->dt, &tz1_Code);
  uint16_t y = year(local_time);
  uint8_t  m = month(local_time);
  uint8_t  d = day(local_time);
  uint8_t  h = hour(local_time);
  int      ip;

  uint8_t icon = moon_phase(y, m, d, h, &ip);

  tft.setTextDatum(TC_DATUM);
  tft.setTextPadding(tft.textWidth("1234567890123")); // Max string length?

  tft.drawString(moonPhase[ip], 120, 302);
  ui.drawBmp("/moon/moonphase_L" + String(icon) + ".bmp", 120 - 30, 318 - 16 - 60);

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextPadding(0); // Reset padding width to none
  tft.drawString(sunStr, 40, 270);

  tft.setTextDatum(BR_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" 88:88 "));

  String rising = strTime(current->sunrise) + " ";
  int dt = rightOffset(rising, ":"); // Draw relative to colon to them aligned
  tft.drawString(rising, 40 + dt, 290);

  String setting = strTime(current->sunset) + " ";
  dt = rightOffset(setting, ":");
  tft.drawString(setting, 40 + dt, 305);

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString(cloudStr, 195, 260);

  String cloudCover = "";
  cloudCover += current->clouds;
  cloudCover += "%";

  tft.setTextDatum(BR_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth(" 100%"));
  tft.drawString(cloudCover, 210, 277);

  tft.setTextDatum(BC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.drawString(humidityStr, 190, 300 - 2);

  String humidity = "";
  humidity += current->humidity;
  humidity += "%";

  tft.setTextDatum(BR_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextPadding(tft.textWidth("100%"));
  tft.drawString(humidity, 210, 315);

  tft.setTextPadding(0); // Reset padding width to none
}

/***************************************************************************************
**                          Get the icon file name from the index number
***************************************************************************************/
const char* getMeteoconIcon(uint16_t id, bool today) {
  if ( today && id / 100 == 8 && (current->dt < current->sunrise || current->dt > current->sunset)) id += 1000;

  if (id / 100 == 2) return "thunderstorm";
  if (id / 100 == 3) return "drizzle";
  if (id / 100 == 4) return "unknown";
  if (id == 500) return "lightRain";
  else if (id == 511) return "sleet";
  else if (id / 100 == 5) return "rain";
  if (id >= 611 && id <= 616) return "sleet";
  else if (id / 100 == 6) return "snow";
  if (id / 100 == 7) return "fog";
  if (id == 800) return "clear-day";
  if (id == 801) return "partly-cloudy-day";
  if (id == 802) return "cloudy";
  if (id == 803) return "cloudy";
  if (id == 804) return "cloudy";
  if (id == 1800) return "clear-night";
  if (id == 1801) return "partly-cloudy-night";
  if (id == 1802) return "cloudy";
  if (id == 1803) return "cloudy";
  if (id == 1804) return "cloudy";

  return "unknown";
}

/***************************************************************************************
**                          Draw messagebox with message
***************************************************************************************/
void messageBox(const char *msg, uint16_t fgcolor, uint16_t bgcolor) {
  messageBox(msg, fgcolor, bgcolor, 5, 240, 230, 24);
}

void messageBox(const char *msg, uint16_t fgcolor, uint16_t bgcolor, int x, int y, int w, int h) {
  uint16_t current_textcolor = tft.textcolor;
  uint16_t current_textbgcolor = tft.textbgcolor;

  tft.loadFont(AA_FONT_SMALL);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(fgcolor, bgcolor);
  tft.fillRoundRect(x, y, w, h, 5, fgcolor);
  tft.fillRoundRect(x + 2, y + 2, w - 4, h - 4, 5, bgcolor);
  tft.setTextPadding(tft.textWidth(msg));
  tft.drawString(msg, w/2, y + (h / 2));
  tft.setTextColor(current_textcolor, current_textbgcolor);
  tft.unloadFont();
}

bool questionBox(const char *msg, uint16_t fgcolor, uint16_t bgcolor, int x, int y, int w, int h) {
  uint16_t current_textcolor = tft.textcolor;
  uint16_t current_textbgcolor = tft.textbgcolor;

  tft.loadFont(AA_FONT_SMALL);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(fgcolor, bgcolor);
  tft.fillRoundRect(x, y, w, h, 5, fgcolor);
  tft.fillRoundRect(x + 2, y + 2, w - 4, h - 4, 5, bgcolor);
  tft.setTextPadding(tft.textWidth(msg));
  tft.drawString(msg, w/2, y + (h / 4));

  tft.fillRoundRect(x + 4, y + (h/2) - 2, (w - 12)/2, (h - 4)/2, 5, TFT_GREEN);
  tft.setTextColor(fgcolor, TFT_GREEN);
  tft.setTextPadding(tft.textWidth("Yes"));
  tft.drawString("Yes", x + 4 + ((w - 12)/4),y + (h/2) - 2 + (h/4));
  tft.fillRoundRect(x + (w/2) + 2, y + (h/2) - 2, (w - 12)/2, (h - 4)/2, 5, TFT_RED);
  tft.setTextColor(fgcolor, TFT_RED);
  tft.setTextPadding(tft.textWidth("No"));
  tft.drawString("No", x + (w/2) + 2 + ((w - 12)/4),y + (h/2) - 2 + (h/4));
  Serial.printf("Yes = x:%d,y:%d,w:%d,h:%d\r\n",x + 4, y + (h/2) - 2, (w - 12)/2, (h - 4)/2);
  Serial.printf("No  = x:%d,y:%d,w:%d,h:%d\r\n",x + (w/2) + 2, y + (h/2) - 2, (w - 12)/2, (h - 4)/2);
  tft.setTextColor(current_textcolor, current_textbgcolor);
  tft.unloadFont();

  uint16_t touchX = 0, touchY = 0;

  long startWhile = millis();
  while (millis()-startWhile<30000) {
    bool pressed = false;
    #ifdef isCYD
      pressed =touchscreen.tirqTouched() && touchscreen.touched();
      if (pressed){
        TS_Point p = touchscreen.getPoint();
        touchX = map(p.x, 170, 3850, 1,240);
        touchY = map(p.y, 310, 3900, 1,320);
        printTouchToSerial(touchX, touchY, p.z);
      }
    #else
      pressed = tft.getTouch(&touchX, &touchY);
    #endif 
    if (pressed){
      Serial.printf("Pressed = x:%d,y:%d\r\n",touchX,touchY);
      if (touchY>=y + (h/2) - 2 && touchY<=y + (h/2) - 2 + ((h - 4)/2)){
        if (touchX>=x + 4 && touchX<=x + 4 + ((w - 12)/2)) return true;
        if (touchX>=x + (w/2) + 2 && touchX<=x + (w/2) + 2 + ((w - 12)/2)) return false;
      }
    }
  }
  return false;
}
/***************************************************************************************
**                          Draw screen section separator line
***************************************************************************************/
// if you don't want separators, comment out the tft-line
void drawSeparator(uint16_t y) {
  tft.drawFastHLine(10, y, 240 - 2 * 10, 0x4228);
}

/***************************************************************************************
**                          Determine place to split a line line
***************************************************************************************/
// determine the "space" split point in a long string
int splitIndex(String text) {
  int index = 0;
  while ( (text.indexOf(' ', index) >= 0) && ( index <= text.length() / 2 ) ) {
    index = text.indexOf(' ', index) + 1;
  }
  if (index) index--;
  return index;
}

/***************************************************************************************
**                          Right side offset to a character
***************************************************************************************/
int rightOffset(String text, String sub) {
  int index = text.indexOf(sub);
  return tft.textWidth(text.substring(index));
}

/***************************************************************************************
**                          Left side offset to a character
***************************************************************************************/
int leftOffset(String text, String sub) {
  int index = text.indexOf(sub);
  return tft.textWidth(text.substring(0, index));
}

/***************************************************************************************
**                          Draw circle segment
***************************************************************************************/
void fillSegment(int x, int y, int start_angle, int sub_angle, int r, unsigned int colour) {
  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x1 = sx * r + x;
  uint16_t y1 = sy * r + y;

  // Draw colour blocks every INCANLGE degrees
  for (int i = start_angle; i < start_angle + sub_angle; i += INCANLGE) {

    // Calculate pair of coordinates for segment end
    int x2 = cos((i + 1 - 90) * DEG2RAD) * r + x;
    int y2 = sin((i + 1 - 90) * DEG2RAD) * r + y;

    tft.fillTriangle(x1, y1, x2, y2, x, y, colour);

    // Copy segment end to segment start for next segment
    x1 = x2;
    y1 = y2;
  }
}

/***************************************************************************************
**                          Print the weather info to the Serial Monitor
***************************************************************************************/
void printWeather(void) {

if (settings.serialMessages){
  Serial.println("Weather from OpenWeather\n");

  Serial.println("############### Current weather ###############\n");
  Serial.print("weatherStation     : "); Serial.println(weatherStation[settings.actualWeatherStation].name);
  Serial.print("dt (time)          : "); Serial.println(strDate(current->dt));
  Serial.print("sunrise            : "); Serial.println(strDate(current->sunrise));
  Serial.print("sunset             : "); Serial.println(strDate(current->sunset));
  Serial.print("main               : "); Serial.println(current->main);
  Serial.print("temp               : "); Serial.println(current->temp);
  Serial.print("humidity           : "); Serial.println(current->humidity);
  Serial.print("pressure           : "); Serial.println(current->pressure);
  Serial.print("wind_speed         : "); Serial.println(windspeedconv(current->wind_speed));
  Serial.print("wind_dir           : "); Serial.println(winddir[calcWindAngle(current->wind_deg)]);
  //Serial.print("wind_dir           : "); Serial.println(calcWindDirection(current->wind_deg));
  Serial.print("clouds             : "); Serial.println(current->clouds);
  Serial.print("id                 : "); Serial.println(current->id);
  Serial.println();

  if (settings.useWapp){
    String loc = "   " + String(weatherStation[settings.actualWeatherStation].name) + String(char(13)) + String(char(10));
    String tim = "Time                  : " + String(strDate(current->dt)) + String(char(13)) + String(char(10));
    String mai = "Main                  : " + String(current->main) + String(char(13)) + String(char(10));
    String tem = "Temp                 : " + String(current->temp) + " oC"  + String(char(13)) + String(char(10));
    String hum = "Humidity           : " + String(current->humidity) + " %" + String(char(13)) + String(char(10));
    String pre = "Pressure           : " + String(current->pressure) + " hPa" + String(char(13)) + String(char(10));
    String win = "Wind speed      : " + String(windspeedconv(current->wind_speed)) + " Bft" + String(char(13)) + String(char(10));
    String wdi = "Wind direction : " + String(winddir[calcWindAngle(current->wind_deg)]) + String(char(13)) + String(char(10));
    String clo = "Clouds              : " + String(current->clouds) + " %" + String(char(13)) + String(char(10));
    String sur = "Sunrise             : "  + String(strDate(current->sunrise)) + String(char(13)) + String(char(10));
    String sus = "Sunset              : "  + String(strDate(current->sunset)) + String(char(13)) + String(char(10));
    tot = loc + tim + mai + tem + hum + pre + win + wdi + clo + sur + sus;
    if (settings.hasLocalTempSensor){
      float tempC = GetLocalTemp();
      String lot;
      if (tempC == -127.00) {
        lot = "Local Temp      : Error"  + String(char(13)) + String(char(10)); 
      }
      else {
        lot = "Local Temp      : "  + String(tempC,1) + " oC" + String(char(13)) + String(char(10)); 
      }
      tot = tot + lot;
    }
  }


  Serial.println("###############  Daily weather  ###############\n");
  Serial.println();

  for (int i = 0; i < 5; i++) {
    Serial.print("dt (time)          : "); Serial.println(strDate(daily->dt[i]));
    Serial.print("id                 : "); Serial.println(daily->id[i]);
    Serial.print("temp_max           : "); Serial.println(daily->temp_max[i]);
    Serial.print("temp_min           : "); Serial.println(daily->temp_min[i]);
    Serial.println();
  }
}

  if (settings.useMQTT){
    if (checkMQTTConnection()) {
      client.publish(String(settings.mqttSubject) + "weather/location", weatherStation[settings.actualWeatherStation].name);
      client.publish(String(settings.mqttSubject) + "weather/dt", strDate(current->dt));
      client.publish(String(settings.mqttSubject) + "weather/sunrise", strDate(current->sunrise));
      client.publish(String(settings.mqttSubject) + "weather/sunset", strDate(current->sunset));
      client.publish(String(settings.mqttSubject) + "weather/main", current->main);
      if (settings.mqttTXUnits){
        client.publish(String(settings.mqttSubject) + "weather/temp", String(current->temp) + " C");
        client.publish(String(settings.mqttSubject) + "weather/humidity", String(current->humidity) + "%");
        client.publish(String(settings.mqttSubject) + "weather/pressure", String(current->pressure) + " mBar");
        client.publish(String(settings.mqttSubject) + "weather/wind_speed", windspeedconv(current->wind_speed) + " Bft");
        client.publish(String(settings.mqttSubject) + "weather/clouds", String(current->clouds) + "%");
      } else {
        client.publish(String(settings.mqttSubject) + "weather/temp", String(current->temp));
        client.publish(String(settings.mqttSubject) + "weather/humidity", String(current->humidity));
        client.publish(String(settings.mqttSubject) + "weather/pressure", String(current->pressure));
        client.publish(String(settings.mqttSubject) + "weather/wind_speed", windspeedconv(current->wind_speed));
        client.publish(String(settings.mqttSubject) + "weather/clouds", String(current->clouds));
      }
      client.publish(String(settings.mqttSubject) + "weather/wind_dir", winddir[calcWindAngle(current->wind_deg)]);
      //client.publish(String(settings.mqttSubject) + "weather/wind_dir",calcWindDirection(current->wind_deg));

    }
  }
}

/***************************************************************************************
**             Convert Unix time to a "local time" time string "12:34"
***************************************************************************************/
String strTime(time_t unixTime) {
  time_t local_time = TIMEZONE.toLocal(unixTime, &tz1_Code);

  String localTime = "";

  if (hour(local_time) < 10) localTime += "0";
  localTime += hour(local_time);
  localTime += ":";
  if (minute(local_time) < 10) localTime += "0";
  localTime += minute(local_time);

  return localTime;
}

/***************************************************************************************
**  Convert Unix time to a local date + time string "Oct 16 17:18", ends with newline
***************************************************************************************/
String strDate(time_t unixTime) {
  time_t local_time = TIMEZONE.toLocal(unixTime, &tz1_Code);

  String localDate = "";

  localDate += monthShortStr(month(local_time));
  localDate += " ";
  localDate += day(local_time);
  localDate += " " + strTime(unixTime);

  return localDate;
}

/***************************************************************************************
**  Convert Windspeed to Beaufort conversion
***************************************************************************************/
String windspeedconv(float windSpeed) {
  String retVal = "";
  if (windSpeed >= 0 && windSpeed < 0.3) retVal = "0";
  if (windSpeed >= 0.3 && windSpeed < 1.6) retVal = "1";
  if (windSpeed >= 1.6 && windSpeed < 3.4) retVal = "2";
  if (windSpeed >= 3.4 && windSpeed < 5.5) retVal = "3";
  if (windSpeed >= 5.5 && windSpeed < 8.0) retVal = "4";
  if (windSpeed >= 8.0 && windSpeed < 10.8) retVal = "5";
  if (windSpeed >= 10.8 && windSpeed < 13.9) retVal = "6";
  if (windSpeed >= 13.9 && windSpeed < 17.2) retVal = "7";
  if (windSpeed >= 17.2 && windSpeed < 20.8) retVal = "8";
  if (windSpeed >= 20.8 && windSpeed < 24.5) retVal = "9";
  if (windSpeed >= 24.5 && windSpeed < 28.5) retVal = "10";
  if (windSpeed >= 28.5 && windSpeed < 32.7) retVal = "11";
  if (windSpeed >= 32.7) retVal = "12";
  return retVal;
}

/***************************************************************************************
**  Show the list of locations
***************************************************************************************/
void locationList() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);

  for (int n = 0; n <= nrOffLocations; n++) {
    messageBox(weatherStation[n].name, TFT_BLUE, TFT_WHITE, (weatherLocation[n].Xlocos + weatherLocation[n].Xlocnr) , (weatherLocation[n].Ylocos) + (weatherLocation[n].Ylocnr), (weatherLocation[n].Xlocsr) , (weatherLocation[n].Ylocsr));
  }
}

/***************************************************************************************
**  Activate selected location
***************************************************************************************/
bool useLocation(bool useLast, uint16_t touchX, uint16_t touchY) {
  bool showLocList = true;
  if (useLast) {
    if (settings.actualWeatherStation >= 0 and settings.actualWeatherStation <= nrOffLocations) showLocList = false;
  } else {
    for (int n = 0; n <= nrOffLocations; n++) {
      if ((touchX > ((weatherLocation[n].Xlocos) + (weatherLocation[n].Xlocnr))) and (touchX < ((weatherLocation[n].Xlocos) + (weatherLocation[n].Xlocsr) + (weatherLocation[n].Xlocnr))) and (touchY > ((weatherLocation[n].Ylocos) + (weatherLocation[n].Ylocnr))) and (touchY < ((weatherLocation[n].Ylocos) + (weatherLocation[n].Ylocsr) + (weatherLocation[n].Ylocnr)))) {
        delay(400);
        showLocList = false;
        settings.actualWeatherStation = n;
      }
    }
  }
  return showLocList;
}

/***************************************************************************************/
void handlePage1(void) {           // side of the band openings

  tft.fillScreen(TFT_BLACK);
  messageBox("Wacht op gegevens", TFT_YELLOW, TFT_NAVY);

  getAndCompressClientData();       // read the main content of the webpage
  drawMufPagePattern();             // draw the basic pattern
  getBandColors();                  // show band colors
  getTopBar();                      // create the top-bar
  getBottomBar();                   // create the bottom-bar
}

/***************************************************************************************/
void drawMufPagePattern(void) {
  tft.fillScreen(B_DD6USB + 3); // a little more intense
  drawWiFiQuality();          // draw top bar WiFi

  for ( int i = 20; i < 280; i += 40)
  {
    tft.drawFastHLine(0, i, 240, TFT_YELLOW);
  }
  tft.drawFastVLine(180, 20, 280, TFT_YELLOW);
  tft.drawFastVLine(122, 20, 200, TFT_YELLOW);
  tft.drawFastVLine( 0, 20, 280, TFT_YELLOW);
  tft.drawFastVLine(239, 20, 280, TFT_YELLOW);
  tft.setFreeFont(&FreeSans12pt7b);
  tft.setCursor(25, 48);
  tft.setTextColor(TFT_YELLOW);
  tft.print("Band");
  tft.setCursor(5, 87);
  tft.print("80m - 40m");
  tft.setCursor(5, 127);
  tft.print("30m - 20m");
  tft.setCursor(5, 167);
  tft.print("17m - 15m");
  tft.setCursor(5, 207);
  tft.print("12m - 10m");
  tft.setCursor(0, 247);
  tft.print(" E - skip 6m [EU]");
  tft.setCursor(0, 287);
  tft.print(" E - skip 4m [EU]");
  //tft.setFreeFont(&FreeMono9pt7b);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(133, 45);
  tft.print("day");
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(183, 45);
  tft.print("night");
  //tft.setTextSize(1);
}

/***************************************************************************************/
void getAndCompressClientData(void) {
  char atom;
  solarBuf = "";                      // clear solarBuf
  String  sbuf = "";
  if (!httpsNet.connect(HAMQSL_HOST, 443)) { //Open Server Connection
    if (settings.isDebug) Serial.println(F("\tHAMQSL_HOST Connection failed"));
    return;
  } else {
    if (settings.isDebug) Serial.println(F("\tHAMQSL_HOST is connected now"));
  }
  httpsNet.print(F("GET "));                       // question to net
  httpsNet.print("/solarxml.php");                 // go to www.hamsql.com/solarxml.php)
  httpsNet.println(F(" HTTP/1.1"));                // send question header
  httpsNet.print(F("HOST: "));
  httpsNet.println(HAMQSL_HOST);
  httpsNet.println(F("Cache-Control: no-cache"));
  if (httpsNet.println() == 0) {
    Serial.println(F("net error"));
    return;
  }
  delay(10);
  char endOfHeaders[] = "\r\n\r\n";                // ignore HTTP Header
  if (!httpsNet.find(endOfHeaders)) {
    Serial.println(F("server error"));
    return;
  }

  //===============>>  now read the webpage  <<==================
  for ( int i = 0; i < maxWebBytes; i++) {
    atom = httpsNet.read();
    sbuf += atom;
  }
  if (settings.isDebug){
    Serial.println(sbuf);
    Serial.print(" len sbuf = ");
    Serial.println(sbuf.length());
  }

  //===============>>  now compress the content  <<==================
  // means: delete all: cr,lf,",space,tab and shortening the string
  String t  = "<updated>"; // begin of "interest" of the content
  String t1 = "</muf>";    //  end  of "interest" of the content
  for (int j = (sbuf.indexOf(t) + t.length()); j < sbuf.indexOf(t1); j++) {
    atom = sbuf[j];
    if (!isWhitespace(atom) && !isControl(atom) && (atom != 34) && (atom != 255))
    {
      solarBuf += atom;     // build the compressed string
    }
  }                            // in solarBuf is now the compessed content-ready for parsing
  printSolarBuf("ClientData");
}

/***************************************************************************************/
void printSolarBuf(String from) {
  if (settings.isDebug){
    Serial.print(from + ": ");
    Serial.println(solarBuf);
    Serial.print(" len solarBuf = ");
    Serial.println(solarBuf.length());
  }
}

/***************************************************************************************/
void getBottomBar(void) {
  String Flux, SN, Muf;
  char buf[40];
  tft.setFreeFont(NULL);;
  tft.fillRoundRect(0, 300, 240, 20, 5, TFT_MAROON);
  tft.drawRoundRect(0, 300, 240, 20, 5, TFT_WHITE);
  tft.fillRoundRect(1, 302, 57, 16, 5, TFT_BLACK);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(2, 306);
  tft.print("tnx:N0NBH");

  Flux = parseString("<solarflux>");
  SN   = parseString("<signalnoise>");
  Muf  = parseString("<muf>");

  if (isAlpha(Muf.charAt(1))) {
    Muf = "noInfo";
  }
  tft.setCursor(51, 306);
  tft.setTextColor(TFT_WHITE);
  sprintf(buf, "  Flux:%s S/N:%s MUF:%s", Flux, SN, Muf);
  tft.print(buf);
  if (settings.isDebug) Serial.println(buf);

  if (settings.useMQTT){
    if (checkMQTTConnection()) {
      client.publish(String(settings.mqttSubject) + "solar/" + "flux", Flux);
      client.publish(String(settings.mqttSubject) + "solar/" + "SN", SN);
      client.publish(String(settings.mqttSubject) + "solar/" + "Muf", Muf);
    }
  }
}

/***************************************************************************************/
void getTopBar(void) {
  tft.setFreeFont(NULL);;
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN);
  char buf[40];
  String date1 = solarBuf.substring(0, 2);   // day
  String date2 = solarBuf.substring(2, 5);   // month
  String date3 = solarBuf.substring(5, 9);   // year
  String date4 = solarBuf.substring(9, 11);  // hour
  String date5 = solarBuf.substring(11, 13); // min
  tft.setCursor(5, 5);
  sprintf(buf, "from %s %s %s %s:%s UTC", date1, date2, date3, date4, date5);
  tft.print(buf);
  tft.drawRoundRect(0, 0, 240, 18, 5, TFT_WHITE);
  if (settings.isDebug) Serial.println(buf);
}

/***************************************************************************************/
void  getBandColors(void) {
  tft.fillRect(125, 63, 53, 35, getMufColor("40mtime=day>", 4));   // 80 day
  tft.fillRect(183, 63, 55, 36, getMufColor("40mtime=night>", 4)); // 80 night
  tft.fillRect(126, 103, 52, 35, getMufColor("20mtime=day>", 4));  // 20 day
  tft.fillRect(183, 103, 54, 35, getMufColor("20mtime=night>", 4)); // 20 night
  tft.fillRect(126, 143, 52, 35, getMufColor("15mtime=day>", 4));  // 15 day
  tft.fillRect(183, 143, 54, 35, getMufColor("15mtime=night>", 4)); // 15 night
  tft.fillRect(126, 183, 52, 35, getMufColor("10mtime=day>", 4));  // 10 day
  tft.fillRect(183, 183, 54, 35, getMufColor("10mtime=night>", 4)); // 10 night
  tft.fillRect(183, 223, 55, 35, getMufColor("_6m>", 6));          //  6
  tft.fillRect(183, 263, 54, 35, getMufColor("_4m>", 6));          //  4
}

/***************************************************************************************/
void handlePage2(int version) {     // side of the MUF-table
  String km_string[8] = {" 100", " 200", " 400", " 600", " 800", "1000", "1500", "3000"};
  uint16_t g;

  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);                       // clear blue screen
  tft.fillRoundRect(0, 0, 240, 16, 5, TFT_MAROON); // build top header
  tft.drawRoundRect(0, 0, 240, 17, 5, TFT_WHITE);
  tft.setTextColor(TFT_CYAN);
  if (version == 0) {                              // get the muf-table-datas
    readIAPData();
  } else {
    readIAPBEData();
  }
  tft.setFreeFont(NULL);;
  tft.setCursor(3, 5);
  tft.print(" " + contentStrings[9] + " UTC "); //show the last change-time
  tft.setTextColor(TFT_WHITE);

  if (version == 0) {                              // get the muf-table-datas
    tft.print(" tnx:iap_kborn.de");
  } else {
    tft.print(" tnx:meteo.be");
  }
  tft.fillRoundRect(3, 19, 234, 298, 5, B_DD6USB + 12);// background
  tft.drawRoundRect(3, 19, 234, 298, 5, TFT_YELLOW); // generate the borders
  tft.setCursor(10, 35);                         // headline position
  tft.setTextColor(TFT_GREEN);                       // headline color
  tft.setFreeFont(&FreeSans9pt7b);                   // headline font
  if (version == 0) {                              // get the muf-table-datas
    tft.print("    Juliusruh  MUF-Table");         // generate headline
    lastIAP = "Juliusruh";
  } else {
    tft.print("    Dourbes  MUF-Table");         // generate headline
    lastIAP = "Dourbes";
  }
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 69);
  tft.println("          km                MHz");// table header
  tft.setFreeFont(&FreeSans12pt7b);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(0, 100);                 // startposition of table-text
  for (g = 0; g < 8; g++)               // print the values in her columns & rows
  {
    tft.print("      " + km_string[g]);
    tft.setCursor(150 , 100 + g * 29);
    if (contentStrings[7 - g].toInt() < 10) tft.print("  "); // arrange positions
    tft.println(contentStrings[7 - g]);        // print the MUF-values
  }
  tft.drawFastVLine(120, 53, 256, TFT_CYAN);      // build column-bar
  tft.drawRect(11, 50, 218, 262, TFT_CYAN);       // generate table-frame
  for (g = 77; g < 300; g += 29)             // build the table-rows
  {
    tft.drawFastHLine(12, g , 216, TFT_CYAN);
  }

  if (settings.useMQTT){
    if (checkMQTTConnection()) {
      for (g = 0; g < 8; g++) {              // print the values in her columns & rows
        String fullSubject = String(settings.mqttSubject) + "muf/";
        if (version == 0) {                              // get the muf-table-datas
          fullSubject += "DE/";
        } else {
          fullSubject += "BE/";
        }
        client.publish(fullSubject + km_string[g], contentStrings[7 - g]);
      }
    }
  }
}

/***************************************************************************************/
void readIAPData(void) {
  char atom;
  String iapBuf = "";                          //clear iapBuf
  if (!httpsNet.connect(IONIAP_HOST, 443)) {      //Open iap-server Connection
  if (settings.isDebug) Serial.println(F("\tIONIAP_HOST Connection failed"));
    return;
  } else {
    if (settings.isDebug) Serial.println(F("\tIONIAP_HOST is connected now"));
  }

  httpsNet.print(F("GET "));                       // question to net
  httpsNet.print("/latest_JR055_MUF.txt");         // go to table-page
  httpsNet.println(F(" HTTP/1.1"));                // send question header
  httpsNet.print(F("HOST: "));
  httpsNet.println(IONIAP_HOST);
  httpsNet.println(F("Cache-Control: no-cache"));  // no old cache please!
  if (httpsNet.println() == 0) {
    if (settings.isDebug) Serial.println(F("net error"));
    return;
  }
  delay(10);
  char endOfHeaders[] = "\r\n\r\n";                // ignore HTTP Header
  if (!httpsNet.find(endOfHeaders)) {
    if (settings.isDebug) Serial.println(F("server error"));
    //return;
  }

  for ( uint16_t i = 0; i < 180; i++) {
    atom = httpsNet.read();
    if (atom != 0xFF) {                            //"forget" the unused atoms
      iapBuf = iapBuf + atom;
    }
  }

if (settings.isDebug) {
  Serial.println("=========================================== ");
  Serial.print(iapBuf);
  Serial.println("=========================================== ");
}

  String t;
  for (uint8_t z = 0; z < 8; z++) {           // parse iap_content
    uint16_t pos = iapBuf.lastIndexOf("\n");  // search for every newline
    t = iapBuf.substring(pos - 5 , pos);      // ectract substring
    iapBuf = iapBuf.substring(0, pos - 5 );   // build iap_substring
    t.trim();                                 // delete whitespace
    contentStrings[z] = t;                    // create iap_fullstring
    if (settings.isDebug) Serial.println("\n " + contentStrings[z]);
  }

  contentStrings[9] = iapBuf.substring(16, 32);  // len = 16 holds: date & time
  contentStrings[9].setCharAt(4, '/');           // change single signs
  contentStrings[9].setCharAt(7, '/');
  contentStrings[9].setCharAt(10, ' ');
  contentStrings[9].setCharAt(13, ':');
  if (settings.isDebug) Serial.println(contentStrings[9]);
}

/***************************************************************************************/
void readIAPBEData(void) {
  char atom;
  String iapBuf = "";                          //clear iapBuf
  if (!httpNet.connect(IONBE_HOST, 80)) {      //Open iap-server Connection
  if (settings.isDebug) Serial.println(F("\tIONBE_HOST Connection failed"));
    return;
  } else {
    if (settings.isDebug) Serial.println(F("\tIONBE_HOST is connected now"));
  }

  httpNet.print(F("GET "));                               // question to net
  httpNet.print("/ionosphere/MUF/latest-MUF-DB049.php");  // go to table-page
  httpNet.println(F(" HTTP/1.1"));                        // send question header
  httpNet.print(F("HOST: "));
  httpNet.println(IONBE_HOST);
  httpNet.println(F("Cache-Control: no-cache"));  // no old cache please!
  if (httpNet.println() == 0) {
    if (settings.isDebug) Serial.println(F("net error"));
    return;
  }
  delay(10);
  char endOfHeaders[] = "\r\n\r\n";                // ignore HTTP Header
  if (!httpNet.find(endOfHeaders)) {
    if (settings.isDebug) Serial.println(F("server error"));
    //return;
  }

  for ( uint16_t i = 0; i < 500; i++) {
    atom = httpNet.read();
    if (atom != 0xFF) {                            //"forget" the unused atoms
      iapBuf = iapBuf + atom;
    }
  }

if (settings.isDebug){
  Serial.println("=========================================== ");
  Serial.print(iapBuf);
  Serial.println("=========================================== ");
}

  uint16_t pos = iapBuf.lastIndexOf("\n");  // Eliminate last line
  iapBuf = iapBuf.substring(0, pos - 5 );

  String t;
  for (uint8_t z = 0; z < 8; z++) {           // parse iap_content
    pos = iapBuf.lastIndexOf("\n");           // search for every newline
    t = iapBuf.substring(pos - 5 , pos);      // ectract substring
    iapBuf = iapBuf.substring(0, pos - 5 );   // build iap_substring
    t.trim();                                 // delete whitespace
    contentStrings[z] = t;                    // create iap_fullstring
    if (settings.isDebug) Serial.println("\n " + contentStrings[z]);
  }

  contentStrings[9] = iapBuf.substring(190, 206);  // len = 16 holds: date & time
  contentStrings[9].setCharAt(4, '/');            // change single signs
  contentStrings[9].setCharAt(7, '/');
  contentStrings[9].setCharAt(10, ' ');
  contentStrings[9].setCharAt(13, ':');
  if (settings.isDebug) Serial.println(contentStrings[9]);
}

/***************************************************************************************/
void handlePage3(void) {  // first side of the solar datas
  bool oddLine = true;
  const char * contentData[] = { "<solarflux>", "<aindex>", "<kindex>",
                           "<kindexnt>", "<xray>", "<sunspots>",
                           "<heliumline>", "<protonflux>", "<electonflux>",
                           "<aurora>", "<normalization>", "<latdegree>"
                         }; // names of content (part 1)
  tft.setFreeFont(NULL);
  tft.fillScreen(B_DD6USB + 7);   // background for the page
  tft.fillRoundRect(0, 0, 240, 20, 5, TFT_MAROON);
  tft.setCursor(180, 5);         // generate the top bar
  tft.setTextColor(TFT_GREEN);
  tft.print("tnx:N0NBH");
  getTopBar();   // show top bar
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(0, 40);          // start displaying the sun values
  for (uint8_t i = 0; i < 12; i++)   // list the content (part 1)
  {
    tft.setTextColor(oddLine ? TFT_YELLOW : TFT_GREEN);
    String t = contentData[i];
    tft.print("  " + t.substring(1, t.length() - 1) + " = ");
    tft.println(parseString(contentData[i]));
    oddLine = !oddLine;
  }

  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 307);
  tft.print("===>>  continued next page"); //hint for page 2

  if (settings.useMQTT){
    if (checkMQTTConnection()) {
      for (uint8_t i = 0; i < 12; i++) {  // list the content (part 1)
        String t = contentData[i];
        t = t.substring(1, t.length() - 1);
        client.publish(String(settings.mqttSubject) + "conditions/"+t, parseString(contentData[i]));
      }
    }
  }
}

/***************************************************************************************/
void handlePage4(void) {  // second side of the solar datas
  bool oddLine = true;
  const char * contentData[] = {"<solarwind>", "<magneticfield>", "<geomagfield>",
                          "<signalnoise>", "<fof2>", "<muffactor>", "<muf>"
                         };   // names of content (part 2)
  tft.setFreeFont(NULL);;
  tft.fillScreen(B_DD6USB + 7);        // background for the page
  tft.fillRoundRect(0, 0, 240, 20, 5, TFT_MAROON);
  tft.setCursor(180, 5);             // generate the top bar
  tft.setTextColor(TFT_GREEN);
  tft.print("tnx:N0NBH");             // thanks PAUL !
  getTopBar();        // show top bar
  tft.setFreeFont(&FreeSans9pt7b);
  tft.setCursor(0, 43);               // start display of the sun values
  for (uint8_t i = 0; i < 7; i++)    // list the content (part 2)
  {
    tft.setTextColor(oddLine ? TFT_YELLOW : TFT_GREEN);
    String t = contentData[i];
    tft.print("  " + t.substring(1, t.length() - 1) + " = ");
    tft.println(parseString(contentData[i]));
    oddLine = !oddLine;
  }
  tft.print("\n   =====>>  EOM  <<===== ");  // END OF MESSAGE !
  tft.setFreeFont(NULL);;
  tft.fillRoundRect(0, 300, 240, 20, 5, TFT_MAROON);
  tft.drawRoundRect(0, 300, 240, 20, 5, TFT_WHITE);
  tft.setTextColor(TFT_CYAN);
  tft.setCursor(15, 306);
  tft.setTextColor(TFT_WHITE);

  if (settings.useMQTT){
    if (checkMQTTConnection()) {
      for (uint8_t i = 0; i < 7; i++) {  // list the content (part 1)
        String t = contentData[i];
        t = t.substring(1, t.length() - 1);
        client.publish(String(settings.mqttSubject) + "conditions/"+t, parseString(contentData[i]));
      }
    }
  }
}

/***************************************************************************************/
String parseString(String s) {  // side of the band openings
  String t = "</" + s.substring(1, s.length()); // parse content-strings
  uint16_t sPos = solarBuf.indexOf(s) + s.length();
  uint16_t ePos = solarBuf.indexOf(t);
  t = solarBuf.substring(sPos, ePos);
  if ((t.length() == 0) || (t == "NoRpt")) t = "NoReport";
  return t;
}

/***************************************************************************************/
int8_t getWifiQuality() { // converts the dBm to a range between 0 and 100%
  int32_t dbm = WiFi.RSSI(); // get WiFi RSSI
  if (dbm <= -100)
  {
    return 0;
  } else if (dbm >= -50)
  {
    return 100;
  } else
  {
    return 2 * (dbm + 100);
  }
}

/***************************************************************************************/
void drawWiFiQuality(void) {
  tft.setFreeFont(NULL);
  char buf[15];
  int8_t quality = getWifiQuality();
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(1);
  tft.setCursor(170, 5);
  sprintf(buf, "RSSI:%2d%c", quality, '%');
  tft.print(buf);
  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (quality > i * 25 || j == 0) {
        tft.drawPixel(224 + 2 * i, 12 - j, TFT_YELLOW);
      }
    }
  }
}

/***************************************************************************************/
String getMufColor(String s, int len, bool asText) {
  int color = getMufColor(s, len);
  if (color ==TFT_RED) return "red";
  if (color ==TFT_YELLOW) return "yellow";
  if (color ==TFT_GREEN) return "green";
  return "white";
}

int getMufColor(String s, int len) {
  uint16_t color, xa;
  xa = solarBuf.indexOf(s) + s.length();
  String t = solarBuf.substring(xa, xa + len);      // one letter is significant

  if (t == "Poor" || t == "BandCl") {         // check "poor"
    color = TFT_RED;
  } else if ( t == "Fair") { // check "fair"
    color = TFT_YELLOW;
  } else if ( t == "Good" || t == "50MHzE" || t == "70MHzE") {  // check "good"
    color = TFT_GREEN;
  }  else {
    color = TFT_WHITE;
  }

  if (settings.useMQTT){
    if (checkMQTTConnection()) client.publish(String(settings.mqttSubject) + "solar/" + s, t);
  }

  return (color);
}

/***************************************************************************************/
bool checkMQTTConnection() {
  if (settings.useMQTT){
    if (!client.connected()) {
      long startWhile = millis();
      while (!client.connect("MQTTGateway", settings.mqttUser, settings.mqttPass) && millis()-startWhile<5000) {
        Serial.print("!");
        delay(1000);
      }
      Serial.println("!");

      if (client.connected()) {
        Serial.print(F("MQTT connected to: "));
        Serial.println(settings.mqttBroker);
        MQTTFailCounter = 0;
        return true;
      } else {
        MQTTFailCounter++;
        if (MQTTFailCounter > 3) settings.useMQTT = false;
        return false;
      } 
    }
  } else return false;
}


//****************************************************************************************************************************************
void sendMessagetoWHATSAPP(String message) { // Send message to WHATSAPP
  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + String(settings.wappPhone) + "&apikey=" + String(settings.wappAPI) + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.println("WHATSAPP Message sent successfully");
  }
  else {
    Serial.println("Error sending the WHATSAPP message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

float GetLocalTemp(){
  sensors.requestTemperatures();
  //tempL = sensors.getTempC(Probe01);
  return sensors.getTempCByIndex(0);   //PA3HK 
}

void PrintLocalTemp() {
  float tempL = GetLocalTemp();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(180, 15);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.print("Binnen");
  if (tempL == -127.00)
  {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.loadFont(AA_FONT_SMALL);
    tft.setCursor(185, 32);
    tft.print("Error");
  }
  else
  {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(180, 32);
    if ((tempL < 10) and (tempL > -10)) tft.print(" ");
    tft.print(tempL, 1);
    tft.print(" oC");
  }
}

void SaveSettings(AsyncWebServerRequest *request){
  if (request->hasParam("wifiSSID")) request->getParam("wifiSSID")->value().toCharArray(settings.wifiSSID,25);
  if (request->hasParam("wifiPass")) request->getParam("wifiPass")->value().toCharArray(settings.wifiPass,25);  
  if (request->hasParam("openWeatherAPI")) request->getParam("openWeatherAPI")->value().toCharArray(settings.openWeatherAPI,35);
  settings.useMQTT = request->hasParam("useMQTT");
  if (request->hasParam("mqttBroker")) request->getParam("mqttBroker")->value().toCharArray(settings.mqttBroker,25); 
  if (request->hasParam("mqttUser")) request->getParam("mqttUser")->value().toCharArray(settings.mqttUser,25);
  if (request->hasParam("mqttPass")) request->getParam("mqttPass")->value().toCharArray(settings.mqttPass,25);
  if (request->hasParam("mqttSubject")) request->getParam("mqttSubject")->value().toCharArray(settings.mqttSubject,25);
  settings.mqttTXUnits = request->hasParam("mqttTXUnits");
  if (request->hasParam("mqttPort")) settings.mqttPort = request->getParam("mqttPort")->value().toInt();
  settings.useWapp = request->hasParam("useWapp");
  if (request->hasParam("wappPhone")) request->getParam("wappPhone")->value().toCharArray(settings.wappPhone,25);
  if (request->hasParam("wappAPI")) request->getParam("wappAPI")->value().toCharArray(settings.wappAPI,35);
  if (request->hasParam("wappInterval")) settings.wappInterval = request->getParam("wappInterval")->value().toInt();
  settings.serialMessages = request->hasParam("serialMessages");
  settings.hasLocalTempSensor = request->hasParam("hasLocalTempSensor");  
  if (request->hasParam("updateInterval")) settings.updateInterval = request->getParam("updateInterval")->value().toInt();
  if (request->hasParam("pageDelay")) settings.pageDelay = request->getParam("pageDelay")->value().toInt();
  settings.reverseRotation = request->hasParam("reverseRotation");
  settings.isDebug = request->hasParam("isDebug");

  if (request->hasParam("actualWeatherStation")) settings.actualWeatherStation = request->getParam("actualWeatherStation")->value().toInt();

  if (request->hasParam("city1")) request->getParam("city1")->value().toCharArray(settings.city1,25);
  if (request->hasParam("latitude1")) request->getParam("latitude1")->value().toCharArray(settings.latitude1,25);
  if (request->hasParam("longitude1")) request->getParam("longitude1")->value().toCharArray(settings.longitude1,25);  

  if (request->hasParam("city2")) request->getParam("city2")->value().toCharArray(settings.city2,25);
  if (request->hasParam("latitude2")) request->getParam("latitude2")->value().toCharArray(settings.latitude2,25);
  if (request->hasParam("longitude2")) request->getParam("longitude2")->value().toCharArray(settings.longitude2,25);    

  if (request->hasParam("city3")) request->getParam("city3")->value().toCharArray(settings.city3,25);
  if (request->hasParam("latitude3")) request->getParam("latitude3")->value().toCharArray(settings.latitude3,25);
  if (request->hasParam("longitude3")) request->getParam("longitude3")->value().toCharArray(settings.longitude3,25);  

  if (request->hasParam("city4")) request->getParam("city4")->value().toCharArray(settings.city4,25);
  if (request->hasParam("latitude4")) request->getParam("latitude4")->value().toCharArray(settings.latitude4,25);
  if (request->hasParam("longitude4")) request->getParam("longitude4")->value().toCharArray(settings.longitude4,25);  
}

void PrintConfig(){
  Serial.printf("wifiSSID: %s\r\n",settings.wifiSSID);
  Serial.printf("wifiPass: %s\r\n",settings.wifiPass);
  Serial.printf("openWeatherAPI: %s\r\n",settings.openWeatherAPI);
  Serial.printf("useMQTT: %s\r\n",settings.useMQTT?"yes":"no");
  Serial.printf("mqttBroker: %s\r\n",settings.mqttBroker);
  Serial.printf("mqttUser: %s\r\n",settings.mqttUser);
  Serial.printf("mqttPass: %s\r\n",settings.mqttPass);
  Serial.printf("mqttSubject: %s\r\n",settings.mqttSubject);
  Serial.printf("mqttUnits: %s\r\n",settings.mqttTXUnits?"yes":"no");
  Serial.printf("mqttPort: %d\r\n",settings.mqttPort);
  Serial.printf("useWhatsApp: %s\r\n",settings.useWapp?"yes":"no");
  Serial.printf("wappPhone: %s\r\n",settings.wappPhone);
  Serial.printf("wappAPI: %s\r\n",settings.wappAPI);
  Serial.printf("wappInterval: %d\r\n",settings.wappInterval);
  Serial.printf("serialMessages: %s\r\n",settings.serialMessages?"yes":"no");
  Serial.printf("hasLocalTempSensor: %s\r\n",settings.hasLocalTempSensor?"yes":"no");
  Serial.printf("updateInterval: %d\r\n",settings.updateInterval);
  Serial.printf("pageDelay: %d\r\n",settings.pageDelay);
  Serial.printf("reverseRotation: %s\r\n",settings.reverseRotation?"yes":"no");
  Serial.printf("isDebug: %s\r\n",settings.isDebug?"yes":"no");

  Serial.printf("actualWeatherStation: %d\r\n",settings.actualWeatherStation);

  Serial.printf("city1: %s\r\n",settings.city1);
  Serial.printf("latitude1: %s\r\n",settings.latitude1);  
  Serial.printf("longitude1: %s\r\n",settings.longitude1);    

  Serial.printf("city2: %s\r\n",settings.city2);
  Serial.printf("latitude2: %s\r\n",settings.latitude2);  
  Serial.printf("longitude2: %s\r\n",settings.longitude2); 

  Serial.printf("city3: %s\r\n",settings.city3);
  Serial.printf("latitude3: %s\r\n",settings.latitude3);  
  Serial.printf("longitude3: %s\r\n",settings.longitude3); 

  Serial.printf("city4: %s\r\n",settings.city4);
  Serial.printf("latitude4: %s\r\n",settings.latitude4);  
  Serial.printf("longitude4: %s\r\n",settings.longitude4); 
}

String processor(const String& var){
  char buf[150];
  if (var == "version") return VERSION;
  if (var == "wifiSSID") return settings.wifiSSID;
  if (var == "wifiPass") return settings.wifiPass;
  if (var == "openWeatherAPI") return settings.openWeatherAPI;
  if (var == "useMQTT") return settings.useMQTT?"checked":"";
  if (var == "mqttBroker") return settings.mqttBroker;
  if (var == "mqttUser") return settings.mqttUser;
  if (var == "mqttPass") return settings.mqttPass;
  if (var == "mqttSubject") return settings.mqttSubject;
  if (var == "mqttTXUnits") return settings.mqttTXUnits?"checked":"";
  if (var == "mqttPort") return String(settings.mqttPort);
  if (var == "useWapp") return settings.useWapp?"checked":"";
  if (var == "wappPhone") return settings.wappPhone;
  if (var == "wappAPI") return settings.wappAPI;
  if (var == "wappInterval") return String(settings.wappInterval);
  if (var == "serialMessages") return settings.serialMessages?"checked":"";
  if (var == "hasLocalTempSensor") return settings.hasLocalTempSensor?"checked":"";  
  if (var == "updateInterval") return String(settings.updateInterval);
  if (var == "pageDelay") return String(settings.pageDelay);
  if (var == "reverseRotation") return settings.reverseRotation?"checked":"";
  if (var == "isDebug") return settings.isDebug?"checked":"";

  if (var == "actualWeatherStation") return String(settings.actualWeatherStation);

  if (var == "city1") return settings.city1;
  if (var == "latitude1") return settings.latitude1;
  if (var == "longitude1") return settings.longitude1;

  if (var == "city2") return settings.city2;
  if (var == "latitude2") return settings.latitude2;
  if (var == "longitude2") return settings.longitude2;

  if (var == "city3") return settings.city3;
  if (var == "latitude3") return settings.latitude3;
  if (var == "longitude3") return settings.longitude3;

  if (var == "city4") return settings.city4;
  if (var == "latitude4") return settings.latitude4;
  if (var == "longitude4") return settings.longitude4;

  if (var == "locationList") return WebLocationList();
  if (var == "location") return weatherStation[settings.actualWeatherStation].name;
  if (var == "dt") return strDate(current->dt).c_str();
  if (var == "sunrise") return strDate(current->sunrise).c_str();
  if (var == "sunset") return strDate(current->sunset).c_str();
  if (var == "main") return current->main.c_str();
  if (var == "localTemp" && settings.hasLocalTempSensor){
    float tempC = GetLocalTemp();
    Serial.println(tempC);
    sprintf(buf, "<tr><td class=\"myRight\">Inhouse:</td><td class=\"myLeft\">%.2f&#176;C</td><td class=\"myLeft\"></td></tr>",tempC);
    Serial.println(buf);
    return buf;
  }
  if (var == "temp") return (String(current->temp)).c_str();
  if (var == "humidity") return (String(current->humidity) + "&#37;").c_str();
  if (var == "pressure") return (String(current->pressure,0) + " hPa").c_str();
  if (var == "wind_speed") return (String(windspeedconv(current->wind_speed)) + " Bft").c_str();
  if (var == "wind_deg") return winddir[calcWindAngle(current->wind_deg)].c_str();
  if (var == "clouds") return (String(current->clouds) + "&#37;").c_str();
  if (var == "weatherIcon"){
    sprintf(buf, "plaatje?image=/icon/%s.bmp",getMeteoconIcon(current->id, true));
    return buf;
  }
  if (var == "wind_degIcon"){
    int windAngle = calcWindAngle(current->wind_deg);
    String wind[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW" };
    sprintf(buf, "plaatje?image=/wind/%s.bmp",wind[windAngle]);
    return buf;
  }

  if (var == "moonText" || var == "moonIcon"){
    time_t local_time = TIMEZONE.toLocal(current->dt, &tz1_Code);
    uint16_t y = year(local_time);
    uint8_t  m = month(local_time);
    uint8_t  d = day(local_time);
    uint8_t  h = hour(local_time);
    int      ip;
    uint8_t icon = moon_phase(y, m, d, h, &ip); 
    Serial.printf("Moonphase:%d\r\n",ip);
    if (var == "moonText") return moonPhase[ip];
    if (var == "moonIcon"){
      sprintf(buf, "plaatje?image=/moon/moonphase_L%s.bmp",String(icon));
      return buf;
    }
    return "";
  }
  if (var == "forcastDay1"){
    String day  = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[0], &tz1_Code))];
    day.toUpperCase();
    sprintf(buf, "%s",day);
    return buf;
  }
  if (var == "forcastDay2"){
    String day  = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[1], &tz1_Code))];
    day.toUpperCase();
    sprintf(buf, "%s",day);
    return buf;
  }
  if (var == "forcastDay3"){
    String day  = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[2], &tz1_Code))];
    day.toUpperCase();
    sprintf(buf, "%s",day);
    return buf;
  }
  if (var == "forcastDay4"){
    String day  = shortDOW[weekday(TIMEZONE.toLocal(daily->dt[3], &tz1_Code))];
    day.toUpperCase();
    sprintf(buf, "%s",day);
    return buf;
  }
  if (var == "minDay1") return String(daily->temp_min[0], 0).c_str();
  if (var == "maxDay1") return String(daily->temp_max[0], 0).c_str();
  if (var == "minDay2") return String(daily->temp_min[1], 0).c_str();
  if (var == "maxDay2") return String(daily->temp_max[1], 0).c_str();
  if (var == "minDay3") return String(daily->temp_min[2], 0).c_str();
  if (var == "maxDay3") return String(daily->temp_max[2], 0).c_str();
  if (var == "minDay4") return String(daily->temp_min[3], 0).c_str();
  if (var == "maxDay4") return String(daily->temp_max[3], 0).c_str();

  if (var == "iconDay1"){
    sprintf(buf, "plaatje?image=/icon/%s.bmp",getMeteoconIcon(daily->id[0], false));
    return buf;
  }
  if (var == "iconDay2"){
    sprintf(buf, "plaatje?image=/icon/%s.bmp",getMeteoconIcon(daily->id[1], false));
    return buf;
  }
  if (var == "iconDay3"){
    sprintf(buf, "plaatje?image=/icon/%s.bmp",getMeteoconIcon(daily->id[2], false));
    return buf;
  }
  if (var == "iconDay4"){
    sprintf(buf, "plaatje?image=/icon/%s.bmp",getMeteoconIcon(daily->id[3], false));
    return buf;
  }
  if (var == "40Day") return String(getMufColor("40mtime=day>", 4, true)).c_str();
  if (var == "40Night") return String(getMufColor("40mtime=night>", 4, true)).c_str();
  if (var == "20Day") return String(getMufColor("20mtime=day>", 4, true)).c_str();
  if (var == "20Night") return String(getMufColor("20mtime=night>", 4, true)).c_str();
  if (var == "15Day") return String(getMufColor("15mtime=day>", 4, true)).c_str();
  if (var == "15Night") return String(getMufColor("15mtime=night>", 4, true)).c_str();
  if (var == "10Day") return String(getMufColor("10mtime=day>", 4, true)).c_str();
  if (var == "10Night") return String(getMufColor("10mtime=night>", 4, true)).c_str();
  if (var == "6Skip") return String(getMufColor("_6m>", 6, true)).c_str();
  if (var == "4Skip") return String(getMufColor("_4m>", 6, true)).c_str();

  if (var == "IAP") return lastIAP.c_str();
  if (var == "100km") return contentStrings[7].c_str();
  if (var == "200km") return contentStrings[6].c_str();
  if (var == "400km") return contentStrings[5].c_str();
  if (var == "600km") return contentStrings[4].c_str();  
  if (var == "800km") return contentStrings[3].c_str();
  if (var == "1000km") return contentStrings[2].c_str();
  if (var == "1500km") return contentStrings[1].c_str();
  if (var == "3000km") return contentStrings[0].c_str();   
  return "";
}

String WebLocationList(){
  char buf[100];
  String s = "<select name=\"locationsList\" id=\"locationsList\" onchange=\"changeActualLocation()\">";
  for (int i = 0;i<6;i++){
    sprintf(buf,"<option value=\"%d\"%s>%02d: %s</option>", i,(settings.actualWeatherStation==i)?" selected ":"",i, weatherStation[i].name);
    s += String(buf);
  }
  s += "</select>";
  s.replace("%","&#37"); //
  return s;
}

/***************************************************************************************
**            EEPROM Routines
***************************************************************************************/
bool SaveConfig() {
  for (unsigned int t = 0; t < sizeof(settings); t++)
    EEPROM.write(offsetEEPROM + t, *((char*)&settings + t));
  EEPROM.commit();
  if (settings.isDebug) Serial.println("Configuration:saved");
  return true;
}

bool LoadConfig() {
  bool retVal = true;
  if (EEPROM.read(offsetEEPROM + 0) == settings.chkDigit){
    for (unsigned int t = 0; t < sizeof(settings); t++)
      *((char*)&settings + t) = EEPROM.read(offsetEEPROM + t);
  } else retVal = false;
  if (settings.isDebug) Serial.println("Configuration:" + retVal?"Loaded":"Not loaded");
  return retVal;
}

bool CompareConfig() {
  bool retVal = true;
  for (unsigned int t = 0; t < sizeof(settings); t++)
    if (*((char*)&settings + t) != EEPROM.read(offsetEEPROM + t)) retVal = false;
  return retVal;
}

/***************************************************************************************
**            Calibrate touch
***************************************************************************************/
void TouchCalibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

  Serial.println();
  Serial.print("calData[5] = { ");
  for (uint8_t i = 0; i < 5; i++) {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }
  Serial.println(" };");
  settings.calData0 = calData[0];
  settings.calData1 = calData[1];
  settings.calData2 = calData[2];
  settings.calData3 = calData[3];
  settings.calData4 = calData[4];
  SaveConfig();

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  delay(2000);
}

#ifdef isCYD
void printTouchToSerial(int touchX, int touchY, int touchZ) {
  Serial.print("X = ");
  Serial.print(touchX);
  Serial.print(" | Y = ");
  Serial.print(touchY);
  Serial.print(" | Pressure = ");
  Serial.print(touchZ);
  Serial.println();
}
#endif