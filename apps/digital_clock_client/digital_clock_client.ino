#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// Import required libraries for esp8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>

void setup() {

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
  }
}

void loop() {

}
