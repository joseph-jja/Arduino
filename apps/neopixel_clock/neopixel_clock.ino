//  ESP32 S3 + RTC and 24 neopixels for a clock
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "Config.h"
#include "Constants.h"

#include <Arduino.h>
#include <RTClib.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <Adafruit_NeoPixel.h>

static int RED[3] = {255, 0, 0}; 
static int LIGHT_RED[3] = {150, 0, 0}; 
static int GREEN[3] = {0, 255, 0}; 
static int LIGHT_GREEN[3] = {0, 150, 0}; 
static int BLUE[3] = {0, 0, 255}; 
static int LIGHT_BLUE[3] = {0, 0, 150}; 
static int YELLOW[3] = {255, 255, 0}; 
static int CYAN[3] = {0, 255, 255}; 
static int PINKISH[3] = {255, 0, 150}; 
static int PURPLE[3] = {255, 0, 255}; 

#define SERIAL_BAUD 115200
#define TIME_BUFFER_SIZE 30

bool debug = true;

bool rtcInitialized = false;

// real time clock
RTC_DS3231 rtc;

static time_t rawtime;
long currentHour = 0,
     currentMinute = 0;

WebServer server(80);

/* 
  Binary Table
  0 0 0 0 = 0
  0 0 0 1 = 1
  0 0 1 0 = 2
  0 0 1 1 = 3
  0 1 0 0 = 4
  0 1 0 1 = 5
  0 1 1 0 = 6
  0 1 1 1 = 7
  1 0 0 0  = 8 
  1 0 0 1 = 9
*/
static int ZERO_NUM[4] = {0, 0, 0, 0};
static int ONE_NUM[4] = {0, 0, 0, 1};
static int TWO_NUM[4] = {0, 0, 1, 0};
static int THREE_NUM[4] = {0, 0, 1, 1};
static int FOUR_NUM[4] = {0, 1, 0, 0};
static int FIVE_NUM[4] = {0, 1, 0, 1};
static int SIX_NUM[4] = {0, 1, 1, 0};
static int SEVEN_NUM[4] = {0, 1, 1, 1};
static int EIGHT_NUM[4] = {1, 0, 0, 0};
static int NINE_NUM[4] = {1, 0, 0, 1};

#define HOURS_ONE 18
#define HOURS_TWO 17
#define MINUTES_ONE 16
#define MINUTES_TWO 15
#define SECONDS_ONE 7
#define SECONDS_TWO 6

#define PIXEL_CT 4

Adafruit_NeoPixel pixels_hours_1 = Adafruit_NeoPixel(PIXEL_CT, HOURS_ONE);
Adafruit_NeoPixel pixels_hours_2 = Adafruit_NeoPixel(PIXEL_CT, HOURS_TWO);
Adafruit_NeoPixel pixels_minutes_1 = Adafruit_NeoPixel(PIXEL_CT, MINUTES_ONE);
Adafruit_NeoPixel pixels_minutes_2 = Adafruit_NeoPixel(PIXEL_CT, MINUTES_TWO);
Adafruit_NeoPixel pixels_seconds_1 = Adafruit_NeoPixel(PIXEL_CT, SECONDS_ONE);
Adafruit_NeoPixel pixels_seconds_2 = Adafruit_NeoPixel(PIXEL_CT, SECONDS_TWO);

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
    memset(buffer, '\0', sizeof(buffer));
    //int year = rtnow.year();
    //int month = rtnow.month();
    //int day = rtnow.day();
    int hour = rtnow.hour();
    int minutes = rtnow.minute();
    //int seconds = rtnow.second();
    // client clock only needs time really
    snprintf(buffer, sizeof(buffer), "%d:%d#", hour, minutes);
    server.send(200, "text/plain", buffer);
  } else {
    server.send(200, "text/plain", "ERROR");
  }
}

void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.println("Application setup!");
  Serial.print("Build date: ");
  Serial.println(__DATE__);
  Serial.print("Build time: ");
  Serial.println(__TIME__);

  pixels_hours_1.begin();
  pixels_hours_1.setBrightness(85); // 1/3 brightness
  pixels_hours_1.show();

  pixels_hours_2.begin();
  pixels_hours_2.setBrightness(85); // 1/3 brightness
  pixels_hours_2.show();

  pixels_minutes_1.begin();
  pixels_minutes_1.setBrightness(85); // 1/3 brightness
  pixels_minutes_1.show();

  pixels_minutes_2.begin();
  pixels_minutes_2.setBrightness(85); // 1/3 brightness
  pixels_minutes_2.show();

  pixels_seconds_1.begin();
  pixels_seconds_1.setBrightness(85); // 1/3 brightness
  pixels_seconds_1.show();

  pixels_seconds_2.begin();
  pixels_seconds_2.setBrightness(85); // 1/3 brightness
  pixels_seconds_2.show();

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

  if (MDNS.begin("esp32-wroom")) {
    Serial.println("MDNS responder started");
  }
  MDNS.addService("_http", "_tcp", 80);
}

void setSinglePixelColor(Adafruit_NeoPixel pixels, int color[], int pixel_index) {
  pixels.setPixelColor(pixel_index, color[0], color[1], color[2]);
}

void setPixelNumber(Adafruit_NeoPixel pixels, int color[], int number[]) {

  // we know number is going to be 4 
  size_t length = sizeof(number) / sizeof(number[0]);
  int end = (int) length;

  for (int i = 0; i < end; i++) {
    int onOrOff = number[i];
    if (0 == onOrOff) {
      // color is black aka off
      pixels.setPixelColor(i, 0, 0, 0);
    } else {
      pixels.setPixelColor(i, color[0], color[1], color[2]);
    }
  }
}

void loop() {

  
}
