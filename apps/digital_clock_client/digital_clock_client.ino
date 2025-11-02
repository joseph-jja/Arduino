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

#include "Config.h"

// TODO figure out which board type devices and pin
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // Example: FC16_HW for common 8x8 modules
#define MAX_DEVICES 4                     // Number of 8x8 matrix modules chained together
#define CS_PIN 8                         // CS pin connected to the MAX7219
#define CLK_PIN 5
#define DATA_PIN 7

#define TIME_BUFFER_SIZE 30

IPAddress gateway;
WiFiClient client;

// Create an instance of the MD_MAX72xx class
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

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
  Serial.println(WiFi.localIP().toString());
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

void writeString(char hours[], char minutes[]) {
  
  mx.clear();

  if (strlen(hours) == 1) {
    mx.setChar(0, '0'); 
    mx.setChar(1, hours[0]); 
  }else {
    mx.setChar(0, hours[0]); 
    mx.setChar(1, hours[1]); 
  }

  if (strlen(minutes) == 1) {
    mx.setChar(2, '0'); 
    mx.setChar(3, minutes[0]); 
  }else {
    mx.setChar(2, minutes[0]); 
    mx.setChar(3, minutes[1]); 
  }
}

void fetch_data() {

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
}

void updateClient() {
  
  // TODO parse string
  // update  rawtime struct
  // update currentHour and currentMinute for display
  char hours[3];
  char minutes[3];
  bool isMinutes = false;
  bool keepChar = true;
  int h = 0, m = 0;
  if (time_buffer != NULL && strlen(time_buffer) > 3) {
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
    Serial.print("Hours ");
    Serial.print(hours);
    Serial.print("and minutes ");
    Serial.println(minutes);
    writeString(hours, minutes);
  }
}

void loop() {

  fetch_data();
  updateClient();
  delay(500);
}
