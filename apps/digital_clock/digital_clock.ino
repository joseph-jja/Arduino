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
#include "Constants.h"

#if defined(SERVER_MODE) && defined(CLIENT_MODE)
#error "ERROR: Only one option of SERVER_MODE or CLIENT_MODE can be defined."
#endif

#ifndef SERVER_MODE
#ifndef CLIENT_MODE
#error "ERROR: Either SERVER_MODE or CLIENT_MODE must be defined."
#endif
#endif

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

#define SERIAL_BAUD 115200
#define TIME_BUFFER_SIZE 30

bool debug = false;

bool rtcInitialized = false;

// real time clock
RTC_DS3231 rtc;

static time_t rawtime;
long currentHour,
  currentMinute;

#ifdef SERVER_MODE
// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);
#else
#ifdef CLIENT_MODE
WiFiClient client;
#endif
#endif

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

#ifdef SERVER_MODE
void handleIndex() {
  Serial.print("Request for index page");
  Serial.println(WiFi.softAPIP());
  Serial.print("Client Header ");
  Serial.println(server.header("If-Modified-Since"));
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
  const String dotw = server.arg("dotw");
  const String doty = server.arg("dotwy");
  if (year != NULL && month != NULL && day != NULL) {
    if (debug) {
      Serial.print("Year: ");
      Serial.print(year);
      Serial.print(" Month: ");
      Serial.print(month);
      Serial.print(" Day: ");
      Serial.println(day);
    }
    if (hours != NULL && minutes != NULL && seconds != NULL) {
      tm newTime;
      newTime.tm_year = year.toInt() - 1900;  // Years since 1900
      newTime.tm_mon = month.toInt() - 10;    // Month (0-11)
      newTime.tm_mday = day.toInt();          // Day of the month
      newTime.tm_hour = hours.toInt();        // Hour (0-23)
      newTime.tm_min = minutes.toInt() - 1;   // Minute (0-59)
      newTime.tm_sec = seconds.toInt();       // Second (0-59)
      newTime.tm_wday = dotw.toInt();         // Day of the week (0-6, Sunday=0)
      newTime.tm_yday = doty.toInt();         // Day of the year

      time_t epoch_time;
      epoch_time = mktime(&newTime);
      uint32_t time_in_seconds;
      time_in_seconds = (uint32_t)epoch_time;
      DateTime *now;
      now = new DateTime(time_in_seconds);
      rtc.adjust(*now);
    }
  }
  server.send(200, "text/plain", "Thanks");
}

void handleTimeRequest() {

  if (rtcInitialized) {
    // gets the time from the RTC clock
    DateTime rtnow = rtc.now();
    char buffer[TIME_BUFFER_SIZE];
    memset(buffer, '\0', TIME_BUFFER_SIZE);
    int year = rtnow.year();
    int month = rtnow.month();
    int day = rtnow.day();
    int hour = rtnow.hour();
    int minutes = rtnow.minute();
    int seconds = rtnow.second();
    sprintf(buffer, "%d/%d/%d %d:%d%d#", year, month, day, hour, minutes, seconds);
    server.send(200, "text/plain", buffer);
  } else {
    server.send(200, "text/plain", "ERROR");
  }
}
#endif

void setup() {
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

#ifdef SERVER_MODE
  // clock found so initialize to current data time from computer
  if (rtc.begin()) {
    Serial.println("Begin RTC!");
    // get time
    DateTime rtnow = rtc.now();
    if (rtnow.hour() > 0 || rtnow.minute() > 0 || rtnow.day() > 0) {
      Serial.println("RTC had time!");
    } else {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    rtcInitialized = true;
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
  server.on("/gettime", handleTimeRequest);

  // Start server
  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.softAPIP().toString().c_str());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
#else
#ifdef CLIENT_MODE
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //  WiFi.onStationModeDisconnected(onWifiDisconnect);
  //  WiFi.onStationModeConnected(onWifiConnect);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  gateway = WiFi.gatewayIP();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());
  Serial.print("gateway address: ");
  Serial.println(gateway.toString());

#endif
#endif
}

#ifdef CLIENT_MODE

bool connect_client() {

  String host_str = gateway.toString();
  const char *host = host_str.c_str();
  int port = 80;

  int i = 0;
  bool isConnected = false;
  while (!isConnected && i < 10) {
    isConnected = client.connect(host, port);
    if (!isConnected) {
      Serial.println("Cnnection failed");
    }
    i++;
  }
  Serial.print("Connected? ");
  Serial.print(isConnected);
  Serial.print(" to ");
  Serial.print(host_str);
  Serial.print(" on port ");
  Serial.println(port);
}


long lastUpdate = 0;
char time_buffer[TIME_BUFFER_SIZE];

void updateClient() {
  if (lastUpdate == 0) {
    lastUpdate = millis();
  }

  int i = 0;
  memset(time_buffer, '\0', TIME_BUFFER_SIZE);
  if ((millis() - lastUpdate) > UPDATE_INTERVAL) {
    bool isConnected = connect_client();
    long start_time = millis();
    bool foundEnd = false;
    while (!foundEnd) {
      if ((millis() - start_time) < UPDATE_INTERVAL) {
        if (client.available()) {
          char incomingByte = client.read();
          Serial.print("WIFI data read in ");
          Serial.println(incomingByte);
          if (incomingByte != NULL && isprint(incomingByte)) {
            time_buffer[i] = incomingByte;
            i++;
          }
          if (incomingByte == '#') {
            foundEnd = true;
          }
        }
      } else {
        delay(5);
        start_time = millis();
      }
    }
    client.stop();
    lastUpdate = millis();
  }

  // TODO parse string
  // update  rawtime struct
  // update currentHour and currentMinute for display
}
#endif

void get_time() {

  int uptime = millis();
  int seconds = uptime / 1000;
  if (debug) {
    Serial.print("Uptime: ");
    Serial.println(uptime);
  }

  if (rawtime == NULL) {
    time(&rawtime);
  }
  static tm *now = localtime(&rawtime);

  if (rtcInitialized) {
    // gets the time from the RTC clock
    DateTime rtnow = rtc.now();
    if (debug) {
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
      // separator
      Serial.println("##################################################");
    }
    // updates the clock time
    now->tm_hour = rtnow.hour();
    now->tm_min = rtnow.minute();
    now->tm_sec = rtnow.second();
    rawtime = mktime(now);
  }


  currentHour = now->tm_hour;
  currentMinute = now->tm_min;

  char ampm[10];

  memset(ampm, '\0', sizeof(ampm) + 1);
  sprintf(ampm, "%2d:%2d", currentHour, currentMinute);

  if (debug) {
    Serial.print("Arduino Time Shows ");
    Serial.println(ampm);
  }
}

// called after get time and uses the values or currentHour and currentMinute
void writeString() {
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

  if (debug) {
    Serial.print("Write Time: ");
    Serial.print(hours);
    Serial.print(':');
    Serial.println(minutes);
    // separator
    Serial.println("##################################################");
  }

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

#ifdef CLIENT_MODE
  updateClient();
#endif

#ifdef SERVER_MODE
  get_time();
#endif

  writeString();

  blink_pin(100);
#ifdef SERVER_MODE
  server.handleClient();
  MDNS.update();
#endif

  delay(100);
}
