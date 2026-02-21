//  ESP32 S3 + RTC and 24 neopixels for a clock
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <Arduino.h>
#include <RTClib.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <Adafruit_NeoPixel.h>

#include "Config.h"
#include "Constants.h"

static int RED[3] = { 255, 0, 0 };
static int LIGHT_RED[3] = { 150, 0, 0 };
static int GREEN[3] = { 0, 255, 0 };
static int LIGHT_GREEN[3] = { 0, 150, 0 };
static int BLUE[3] = { 0, 0, 255 };
static int LIGHT_BLUE[3] = { 0, 0, 150 };
static int YELLOW[3] = { 255, 255, 0 };
static int CYAN[3] = { 0, 255, 255 };
static int PINKISH[3] = { 255, 0, 150 };
static int PURPLE[3] = { 255, 0, 255 };

#define SERIAL_BAUD 115200
#define TIME_BUFFER_SIZE 30

bool debug = true;

bool rtcInitialized = false;

// real time clock
RTC_DS3231 rtc;

static time_t rawtime;
long currentHour = 0,
     currentMinute = 0,
     currentSeconds = 0;

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
static int ZERO_NUM[4] = { 0, 0, 0, 0 };
static int ONE_NUM[4] = { 0, 0, 0, 1 };
static int TWO_NUM[4] = { 0, 0, 1, 0 };
static int THREE_NUM[4] = { 0, 0, 1, 1 };
static int FOUR_NUM[4] = { 0, 1, 0, 0 };
static int FIVE_NUM[4] = { 0, 1, 0, 1 };
static int SIX_NUM[4] = { 0, 1, 1, 0 };
static int SEVEN_NUM[4] = { 0, 1, 1, 1 };
static int EIGHT_NUM[4] = { 1, 0, 0, 0 };
static int NINE_NUM[4] = { 1, 0, 0, 1 };

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
  pixels_hours_1.setBrightness(85);  // 1/3 brightness
  pixels_hours_1.show();

  pixels_hours_2.begin();
  pixels_hours_2.setBrightness(85);  // 1/3 brightness
  pixels_hours_2.show();

  pixels_minutes_1.begin();
  pixels_minutes_1.setBrightness(85);  // 1/3 brightness
  pixels_minutes_1.show();

  pixels_minutes_2.begin();
  pixels_minutes_2.setBrightness(85);  // 1/3 brightness
  pixels_minutes_2.show();

  pixels_seconds_1.begin();
  pixels_seconds_1.setBrightness(85);  // 1/3 brightness
  pixels_seconds_1.show();

  pixels_seconds_2.begin();
  pixels_seconds_2.setBrightness(85);  // 1/3 brightness
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

void get_time() {

  // no time no call function :)
  if (!rtcInitialized) {
    return;
  }

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
    Serial.print(rtnow.hour(), DEC);  // hour is in 24 hour time format
    Serial.print(':');
    Serial.print(rtnow.minute(), DEC);
    Serial.print(':');
    Serial.println(rtnow.second(), DEC);
  }
  // updates the clock time
  now->tm_hour = rtnow.hour();
  now->tm_min = rtnow.minute();
  now->tm_sec = rtnow.second();
  rawtime = mktime(now);

  currentHour = now->tm_hour;
  currentMinute = now->tm_min;
  currentSeconds = now->tm_sec;

  if (debug) {
    char ampm[20];

    memset(ampm, '\0', sizeof(ampm));
    snprintf(ampm, sizeof(ampm), "%2d:%2d", currentHour, currentMinute);

    Serial.print("Arduino Time Shows ");
    Serial.println(ampm);
    Serial.println("##################################################");
  }
}

void setSinglePixelColor(Adafruit_NeoPixel pixels, int color[], int pixel_index) {
  pixels.setPixelColor(pixel_index, color[0], color[1], color[2]);
}

void setPixelNumber(Adafruit_NeoPixel pixels, int color[], int number[]) {

  // we know number is going to be 4
  size_t length = sizeof(number) / sizeof(number[0]);
  int end = (int)length;

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

void copy_number(int binary_number[], int *input_number) {

  size_t length = sizeof(binary_number) / sizeof(binary_number[0]);
  int end = (int)length;

  for (int i = 0; i < end; i++) {
    binary_number[i] = input_number[i];
  }
}

void decimal_to_binary(int binary_number[], int decimal_number) {

  switch (decimal_number) {
    case 0:
      copy_number(binary_number, ZERO_NUM);
      break;
    case 1:
      copy_number(binary_number, ONE_NUM);
      break;
    case 2:
      copy_number(binary_number, TWO_NUM);
      break;
    case 3:
      copy_number(binary_number, THREE_NUM);
      break;
    case 4:
      copy_number(binary_number, FOUR_NUM);
      break;
    case 5:
      copy_number(binary_number, FIVE_NUM);
      break;
    case 6:
      copy_number(binary_number, SIX_NUM);
      break;
    case 7:
      copy_number(binary_number, SEVEN_NUM);
      break;
    case 8:
      copy_number(binary_number, EIGHT_NUM);
      break;
    case 9:
      copy_number(binary_number, NINE_NUM);
      break;
    default:
      // do nothing and use 0
      break;
  }
}

void loop() {

  get_time();

  int hour_big_number[4];
  int hour_small_number[4];
  int minute_big_number[4];
  int minute_small_number[4];
  int second_big_number[4];
  int second_small_number[4];

  if (currentHour >= 10) {
    int small = currentHour - 10;
    decimal_to_binary(hour_big_number, 1);
    decimal_to_binary(hour_small_number, small);
  } else {
    decimal_to_binary(hour_big_number, 0);
    decimal_to_binary(hour_small_number, currentHour);
  }
  if (currentMinute >= 10) {
    int big = floor(currentMinute / 10);
    int small = currentMinute - 10;
    decimal_to_binary(minute_big_number, big);
    decimal_to_binary(minute_small_number, small);
  } else {
    decimal_to_binary(minute_big_number, 0);
    decimal_to_binary(minute_small_number, currentMinute);
  }
  if (currentSeconds >= 10) {
    int big = floor(currentSeconds / 10);
    int small = currentSeconds - 10;
    decimal_to_binary(second_big_number, big);
    decimal_to_binary(second_small_number, small);
  } else {
    decimal_to_binary(second_big_number, 0);
    decimal_to_binary(second_small_number, currentSeconds);
  }
}
