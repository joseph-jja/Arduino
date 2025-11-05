#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Import required libraries for esp8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>

#include "Config.h"

// You'll need to figure out which board type devices and pin
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  //.1818583410 // Example: FC16_HW for common 8x8 modules
#define MAX_DEVICES 4                      // Number of 8x8 matrix modules chained together
#define CS_PIN D8                          // CS pin connected to the MAX7219
#define CLK_PIN D5                         // CLK pin connected to the MAX7219
#define DATA_PIN D7                        // Data pin connected to the MAX7219

#define TIME_BUFFER_SIZE 30

IPAddress gateway;
WiFiClient client;

// Create an instance of the MD_MAX72xx class
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

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

void setup() {

  Serial.begin(9600);
  delay(25);

  setup_builtin_pin();

  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.onStationModeDisconnected(onWifiDisconnect);
  //WiFi.onStationModeConnected(onWifiConnect);

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
  Serial.println();
  Serial.print("gateway address: ");
  Serial.println(gateway.toString());

  for (int i = 0; i < 5; i++) {
    blink_pin(50);
    delay(25);
  }

  // Initialize the LED matrix
  mx.begin();
  //mx.setIntensity(0); // Set brightness (0-15)
  mx.clear();
}

long lastUpdate = 0;
char time_buffer[TIME_BUFFER_SIZE];

void writeString(char hours[], char minutes[]) {

  mx.clear();

  //Serial.print("Columns: ");
  //Serial.println(mx.getColumnCount());

  if (strlen(hours) == 1) {
    mx.setChar(30, '0');
    mx.setChar(22, hours[0]);
  } else {
    mx.setChar(30, hours[0]);
    mx.setChar(22, hours[1]);
  }

  if (strlen(minutes) == 1) {
    mx.setChar(14, '0');
    mx.setChar(6, minutes[0]);
  } else {
    mx.setChar(14, minutes[0]);
    mx.setChar(6, minutes[1]);
  }
}

int loop_minutes = 0;
int subminutes = 0;
bool firstTime = true;
void fetch_data() {

  if (lastUpdate == 0) {
    lastUpdate = millis();
  }

  //Serial.println("Attempting to fetch data from remote clock");
  if ((millis() - lastUpdate) > UPDATE_INTERVAL || firstTime) {
    memset(time_buffer, '\0', TIME_BUFFER_SIZE);
    if (WiFi.status() == WL_CONNECTED) {
      firstTime = false;
      char url[128];
      // \r\n\r\n
      sprintf(url, "http://%s/gettime", gateway.toString().c_str());
      Serial.print("Attempting to connect to: ");
      Serial.println(url);

      HTTPClient http;
      http.begin(client, url);
      int httpCode = http.GET();

      if (httpCode == 200) {
        Serial.print("Got success code: ");
        Serial.println(httpCode);
        String payload = http.getString();
        Serial.print("And got: ");
        Serial.println(payload);
        if (payload.length() < 20) { 
          sprintf(time_buffer, payload.c_str());
          loop_minutes = 0;
          subminutes = 0;
        }
      } else {
        Serial.print("Error response code: ");
        Serial.println(httpCode);
      }

      http.end();

      Serial.println("Request made!");
    }
    lastUpdate = millis();
  }
  //Serial.print("Got data?");
}

void updateClient() {

  // TODO parse string
  // update  rawtime struct
  // update currentHour and currentMinute for display
  char hours[3] = {0};
  char minutes[3] = {0};
  bool isMinutes = false;
  bool keepChar = true;
  int h = 0, m = 0;
  if (time_buffer != nullptr && strlen(time_buffer) > 3) {
    for (int i = 0; i < strlen(time_buffer) - 1; i++) {
      if (time_buffer[i] == ':' || time_buffer[i] == '#') {
        isMinutes = true;
        keepChar = false;
      }
      if (keepChar && isMinutes) {
        minutes[m] = time_buffer[i];
        m++;
      } else if (keepChar) {
        hours[h] = time_buffer[i];
        h++;
      }
      keepChar = true;
    }
    int guess_minutes = atoi(minutes) + loop_minutes;
    int guess_hours = atoi(hours);
    if (guess_minutes > 59) {
      guess_minutes = 0;
      guess_hours++;
      if (guess_hours > 23) {
        guess_hours = 0;
      }
    }

    char updated_minutes[3] = {0};
    char updated_hours[3] = {0};
    sprintf(updated_minutes, "%d", guess_minutes);
    sprintf(updated_hours, "%d", guess_hours);

    Serial.print("Hours ");
    Serial.print(hours);
    Serial.print(" and updated hours ");
    Serial.print(updated_hours);
    Serial.print(" and minutes ");
    Serial.print(minutes);
    Serial.print(" and loop minutes ");
    Serial.print(loop_minutes);
    Serial.print(" and updated minutes ");
    Serial.println(updated_minutes);
    
    writeString(updated_hours, updated_minutes);
  }
}

void update_time() {

   if (subminutes < 3) {
    subminutes++;
   } else {
    loop_minutes++;
    subminutes = 0;
   }
}

void loop() {

  fetch_data();
  updateClient();
  delay(15000);
  update_time(); 
}
