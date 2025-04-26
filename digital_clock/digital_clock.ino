#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Import required libraries for esp8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <RTClib.h>

#include "Config.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

#define SERIAL_BAUD 115200

// real time clock
RTC_PCF8523 rtc;

// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);

static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Update Time</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
     font-family: Helvetica, Times, Arial;
     margin: 0px auto;
    }
  </style>
</head>
<body>
  <h1>Update Time</h1>
  <script type="text/javascript">
    const now = new Date(); 
    const dateStamp = `year=${now.getFullYear()}&month=${now.getMonth() + 1}&day=${now.getdate()}`; 
  </script>
</body>
</html>)rawliteral";

void setup_builtin_pin() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Pin enabled");
    digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

void handleIndex() {
    Serial.print("Request for index page");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.softAPIP());
    server.send(200, "text/html", index_html);
    Serial.print("Request for index page");
}

void handleUpdate() {
    const String year = server.arg("year");
    const String month = server.arg("month");
    const String day = server.arg("day");
    const String hours = server.arg("hour");
    const String minutes = server.arg("minutes");
    const String seconds = server.arg("seconds");
    if (year != NULL && month != NULL && day != NULL) {
        Serial.print("Year: ");
        Serial.print(year);
        Serial.print(" Month: ");
        Serial.print(month);
        Serial.print(" Day: ");
        Serial.println(day);
        if (hours != NULL && minutes != NULL && seconds != NULL) {
            //RTCTime(int atoi(day), Month _m, int atoi(year), int atoi(hours), int atoi(minutes), int atoi(seconds), DayOfWeek _dof, SaveLight _sl);
        }    
    }    
    server.send(200, "text/plain", "Thanks");
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println();
    Serial.println("Application setup!");
    Serial.print("Build date: ");
    Serial.println(__DATE__);
    Serial.print("Build time: ");
    Serial.println(__TIME__);

    setup_builtin_pin();

    // it does not seem to work unless you pass in the address?
    alpha4.begin(0x70);
    alpha4.setBrightness(5);

    for (int i = 0; i < 4; i++) {
        alpha4.writeDigitRaw(i, 0xFFFF);
        alpha4.writeDisplay();
    }
  
    delay(1000);
    for (int i = 0; i < 4; i++) {
        alpha4.writeDigitRaw(i, 0x0);
        alpha4.writeDisplay();
    }
    Serial.println("Display initialized!");

    // clock found so initialize to current data time from computer
    if (rtc.begin()) {
        Serial.print("Begin RTC!");
        if (!rtc.initialized()) {
            Serial.print("Not initialized!");
        }
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    } else {
        Serial.println("Could NOT Begin RTC!");        
    }

    Serial.println("Setting up Access Point");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("WiFi with IP address: ");
    Serial.println(IP);


    // Routes for web server
    server.on("/", handleIndex);
    server.on("/update", handleUpdate);

    // Start server
    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.softAPIP().toString().c_str());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
}

static time_t rawtime;
long currentHour, 
   currentMinute;

void get_time() {

    int uptime = millis();
    int seconds = uptime/1000;
    Serial.print("Uptime: ");
    Serial.println(uptime);

    if (rawtime == NULL) { 
        time (&rawtime);
    }
    static tm *now = localtime(&rawtime);
    currentHour = now->tm_hour;
    currentMinute = now->tm_min;

    char ampm[10];

    memset(ampm, '\0', sizeof(ampm) + 1);
    sprintf(ampm, "%2d:%2d", currentHour, currentMinute);

    Serial.print("Arduino Time Shows ");
    Serial.println(ampm);
    
    // no rtc so just pull time from thin air
    if (!rtc.begin()) {
        return;
    }
  
    if (!rtc.initialized()) {
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    // gets the time from the RTC clock
    DateTime rtnow = rtc.now();
    Serial.print("RTC Time Shows ");
    Serial.print(rtnow.year(), DEC);
    Serial.print('/');
    Serial.print(rtnow.month(), DEC);
    Serial.print('/');
    Serial.print(rtnow.day(), DEC);
    Serial.print(' ');
    Serial.print(rtnow.hour(), DEC);
    Serial.print(':');
    Serial.print(rtnow.minute(), DEC);
    Serial.print(':');
    Serial.println(rtnow.second(), DEC);

    // updates the clock time 
    now->tm_hour = rtnow.hour();
    now->tm_min = rtnow.minute();
    now->tm_sec = rtnow.second();
    rawtime = mktime ( now );

    // separator
    Serial.println("##################################################");
}

// called after get time and uses the values or currentHour and currentMinute
void writeString()
{
    char hours[3];
    char minutes[3];

    memset(hours, '\0', sizeof(hours) + 1);
    memset(minutes, '\0', sizeof(minutes) + 1);

    if (currentHour < 10) {
        sprintf(hours, "0%d", currentHour);
    } else {
        sprintf(hours, "%d", currentHour);
    }

    if (currentMinute < 10) {
        sprintf(minutes, "0%d", currentMinute);
    } else {
        sprintf(minutes, "%d", currentMinute);
    }

    Serial.print("Write Time: ");
    Serial.print(hours);
    Serial.print(':');
    Serial.println(minutes);
    // separator
    Serial.println("##################################################");
    
    alpha4.clear();
    alpha4.writeDigitAscii(0, hours[0], false);
    alpha4.writeDigitAscii(1, hours[1], true);
    alpha4.writeDigitAscii(2, minutes[0], false);
    alpha4.writeDigitAscii(3, minutes[1], false);
    alpha4.writeDisplay();
    
    delay(1000);
}

void loop() {

    char buff[255];

    //get_time();

    //writeString();
  
    blink_pin(100);

    server.handleClient();
    MDNS.update();

    delay(100);
}
