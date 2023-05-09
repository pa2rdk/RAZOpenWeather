const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Weather server %location%</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!-- <meta http-equiv="refresh" content="1">  -->
  <link rel="icon" href="data:,">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
  <div class="topnav">
    <h1>Weather Server</h1><br>
    <h1>%location%</h1>
  </div>
  <hr>
  <div class="divinfo">
    <h2>Weather</h2>
    <table>
        <tr>
            <td class="myRight">
                Date:
            </td>
            <td class="myLeft">
                %dt%
            </td>
            <td>
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Sunrise:
            </td>
            <td class="myLeft">
                %sunrise%
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Sunset:
            </td>
            <td class="myLeft">
                %sunset%
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Main:
            </td>
            <td class="myLeft">
                %main%
            </td>
            <td class="myLeft">
                <img src="%weatherIcon%">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Temp:
            </td>
            <td class="myLeft">
                %temp%&#176;C
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Humidity:
            </td>
            <td class="myLeft">
                %humidity%
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Pressure:
            </td>
            <td class="myLeft">
                %pressure%
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                wind:
            </td>
            <td class="myLeft">
                %wind_speed% &nbsp; %wind_deg%
            </td>
            <td class="myLeft">
                <img src="%wind_degIcon%">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Clouds:
            </td>
            <td class="myLeft">
                %clouds%
            </td>
            <td class="myLeft">
            </td>
        </tr>

        <tr>
            <td class="myRight">
                Moon:
            </td>
            <td class="myLeft">
                %moonText%
            </td>
            <td class="myLeft">
                <img src="%moonIcon%">
            </td>
        </tr>

    </table>
  </div>
  <hr>
  <div class="divinfo">
    <h2>Forecast</h2>
    <table style="border: 1px;">
        <tr style="color:yellow">
            <td colspan="2">
                %forcastDay1%
            </td>
            <td colspan="2">
                %forcastDay2%
            </td>
            <td colspan="2">
                %forcastDay3%
            </td>
            <td colspan="2">
                %forcastDay4%
            </td>
        </tr>
        <tr style="color:white">
            <td colspan="2">
                Max Min
            </td>
            <td colspan="2">
                Max Min
            </td>
            <td colspan="2">
                Max Min
            </td>
            <td colspan="2">
                Max Min
            </td>
        </tr>
        <tr style="color:red">
            <td>
              %maxDay1%&#176;C
            </td>
            <td>
              %minDay1%&#176;C
            </td>
            <td>
              %maxDay2%&#176;C
            </td>
            <td>
              %minDay2%&#176;C
            </td>
            <td>
              %maxDay3%&#176;C
            </td>
            <td>
              %minDay3%&#176;C
            </td>
            <td>
              %maxDay4%&#176;C
            </td>
            <td>
              %minDay4%&#176;C
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <img src="%iconDay1%">
            </td>
            <td colspan="2">
                <img src="%iconDay2%">
            </td>
            <td colspan="2">
                <img src="%iconDay3%">
            </td>
            <td colspan="2">
                <img src="%iconDay4%">
            </td>
        </tr>
      </table>
  </div>
  <hr>
  <div class="divinfo">
    <h2>Conditions</h2>
    <table style="border: 1px;">
        <tr style="color:yellow">
            <th>Band</th>
            <th>Day</th>
            <th>Night</th>
        </tr>
        <tr>
            <td>
                80m-40m
            </td>
            <td style="background-color:%40Day%"></td>
            <td style="background-color:%40Night%"></td>
        </tr>
        <tr>
            <td>
                30m-20m
            </td>
            <td style="background-color:%20Day%"></td>
            <td style="background-color:%20Night%"></td>
        </tr>
        <tr>
            <td>
                17m-15m
            </td>
            <td style="background-color:%15Day%"></td>
            <td style="background-color:%15Night%"></td>
        </tr>
        <tr>
            <td>
                12m-10m
            </td>
            <td style="background-color:%10Day%"></td>
            <td style="background-color:%10Night%"></td>
        </tr>
        <tr>
            <td colspan="2">
                E - skip 6m[EU]
            </td>
            <td style="background-color:%6Skip%"></td>
        </tr>
        <tr>
            <td colspan="2">
                E - skip 4m[EU]
            </td>
            <td style="background-color:%4Skip%"></td>
        </tr>
    </table>
</div>
<hr>
<div class="divinfo">
    <h2>MUF %IAP%</h2>
    <table style="border: 1px;">
        <tr style="color:yellow">
            <th>KM</th>
            <th>MHz</th>
        </tr>
        <tr>
            <td>
                100km
            </td>
            <td>%100km%</td>
        </tr>
        <tr>
            <td>
                200km
            </td>
            <td>%200km%</td>
        </tr>
        <tr>
            <td>
                400km
            </td>
            <td>%400km%</td>
        </tr>
        <tr>
            <td>
                600km
            </td>
            <td>%600km%</td>
        </tr>
        <tr>
            <td>
                800km
            </td>
            <td>%800km%</td>
        </tr>
        <tr>
            <td>
                1000km
            </td>
            <td>%1000km%</td>
        </tr>
        <tr>
            <td>
                1500km
            </td>
            <td>%1500km%</td>
        </tr>
        <tr>
            <td>
                3000km
            </td>
            <td>%3000km%</td>
        </tr>
    </table>
</div>
  <div class="topnav" style="background-color: lightblue; ">
    <table>
        <tr>
          <td style="text-align:left; color:black;">
            <h4>PI4RAZ</h4>
          </td>
          <td style="text-align:right;">
            <span id="stationList">%locationList%</span>
            <a href="/settings"><button class="button">Settings</button></a>
            <a href="/reboot"><button class="button">Reboot</button></a>
          </td>
        </tr>
    </table>
  </div>
</body>

<script>
  // alert("Oliebol");

  function changeActualLocation(){
    var e = document.getElementById("locationsList");
    window.location.href = "/golocation?location="+e.value.toString();
  }
  </script>
</html>
)rawliteral";

const char settings_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
  <title>RAZ Weatherstation</title>
  <link rel="icon" href="data:,">
  <meta name=viewport content="width=device-width, initial-scale=1, user-scalable=yes, charset=utf-8">
  <link rel="stylesheet" type="text/css" href="/style.css">
  </head>
  <body>
    <div class="topnav">
      <h1>Weather Server</h1><br>
      <h1>%location%</h1>
      <br>
      <h2>Settings</h2>
    </div>
    <hr>
    <div class="divinfo">
      <form action="/store" method="get">

      <div class="divinfo">
        <table>
          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              WiFi SSID: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="wifiSSID" value="%wifiSSID%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              WiFi Password:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="password" name="wifiPass" value="%wifiPass%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              OpenWeather APIkey: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="openWeatherAPI" value="%openWeatherAPI%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium;">
              Enable MQTT:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="checkbox" name="useMQTT" value="useMQTT" %useMQTT%>
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              MQTT Broker: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="mqttBroker" value="%mqttBroker%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              MQTT User: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="mqttUser" value="%mqttUser%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              MQTT Password: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="password" name="mqttPass" value="%mqttPass%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              MQTT Subject: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="mqttSubject" value="%mqttSubject%">
            </td>
          </tr>
        
          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              MQTT Port: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="mqttPort" value="%mqttPort%">
            </td>
          </tr>        

          <tr>
            <td style="text-align:right;font-size: medium;">
              Enable WhatsApp:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="checkbox" name="useWapp" value="useWapp" %useWapp%>
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              WAPP PhoneNumber: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="wappPhone" value="%wappPhone%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              WAPP APIkey: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="wappAPI" value="%wappAPI%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              WAPP Interval: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="wappInterval" value="%wappInterval%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium;">
              Serial messages:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="checkbox" name="serialMessages" value="serialMessages" %serialMessages%>
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium;">
              Has local temperture sensor:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="checkbox" name="hasLocalTempSensor" value="hasLocalTempSensor" %hasLocalTempSensor%>
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              Update Interval: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="updateInterval" value="%updateInterval%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              Page delay: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="pageDelay" value="%pageDelay%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              Actual weatherstation: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="actualWeatherStation" value="%actualWeatherStation%">
              0 = Noordpool, 1 = Zuidpool
            </td>

          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              City2/Lat/Lon: 
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="city1" value="%city1%">
              <input type="text" name="latitude1" value="%latitude1%">
              <input type="text" name="longitude1" value="%longitude1%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              City3/Lat/Lon:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="city2" value="%city2%">
              <input type="text" name="latitude2" value="%latitude2%">
              <input type="text" name="longitude2" value="%longitude2%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              City4/Lat/Lon:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="city3" value="%city3%">
              <input type="text" name="latitude3" value="%latitude3%">
              <input type="text" name="longitude3" value="%longitude3%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium; color: white">
              City5/Lat/Lon:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="text" name="city4" value="%city4%">
              <input type="text" name="latitude4" value="%latitude4%">
              <input type="text" name="longitude4" value="%longitude4%">
            </td>
          </tr>

          <tr>
            <td style="text-align:right;font-size: medium;">
              Debugmode:
            </td>
            <td style="text-align:left;font-size: medium;">
              <input type="checkbox" name="isDebug" value="isDebug" %isDebug%>
            </td>
          </tr>

          <tr>
            <td></td>
            <td style="text-align:left;font-size: medium;">
              <input style="font-size: medium" type="submit" value="Submit">
            </td>
          </tr>
        </table>
      </div>
      
    </form><br>
  </div>
  <hr>

  <div class="topnav" style="background-color: lightblue; ">
    <table>
        <tr>
          <td style="text-align:left; color:black">
            <h4>PI4RAZ</h4>
          </td>
          <td style="text-align:right">
            <a href="/"><button class="button">Main</button></a>
            <a href="/reboot"><button class="button">Reboot</button></a>
          </td>
        </tr>
    </table>
  </div>

  <script>
  if(typeof window.history.pushState == 'function') {
    if (window.location.href.lastIndexOf('/command')>10 || window.location.href.lastIndexOf('/reboot')>10 || window.location.href.lastIndexOf('/store')>10){
      console.log(window.location.href);
      window.location.href =  window.location.href.substring(0,window.location.href.lastIndexOf('/'))
    }
  }
  </script>

  </body>
  </html>
)rawliteral";

const char css_html[] PROGMEM = R"rawliteral(
html {
    font-family: Arial; 
    display: inline-block; 
    text-align: center;
    background-color: black; 
}
p { 
    font-size: 1.2rem;
}

table {
    border-width: 1px;
    width:80%; 
    text-align:center; 
    font-size: large;
    margin-left: auto;
    margin-right: auto;
}

tr {
    height: 25px;
}

body {  
    margin: 0;
}

.myRight {
    text-align: right;
    color: yellow;
    align-items: center;
}

.myLeft {
    text-align: left;
    color:red;
    align-items: center;
}

.topnav { 
    overflow: hidden; 
    background-color: blue; 
    color: white; 
    font-size: 1rem; 
    line-height: 0%;
}
.divinfo { 	
    overflow: hidden; 
    background-color: black; 
    color: white; 
    font-size: 0.7rem; 
    height:100;
    line-height: 0%;
}
.freqinfo { 	
    overflow: hidden; 
    background-color: black;
    color: yellow; 
    font-size: 1rem; 
    line-height: 0%;
}
.content { 
    padding: 20px; 
}
.card { 
    background-color: lightblue; 
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); 
    font-size: 0.7rem; 
}
.cards { 
    max-width: 1000px; 
    margin: 0 auto; 
    display: grid; 
    grid-gap: 1rem; 
    grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); 
}
.reading { 
    font-size: 1.4rem;  
}
})rawliteral";

