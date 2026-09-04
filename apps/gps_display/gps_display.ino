// designed for the ESP32 S3 dual core!
// ESP32 S3 -> Development Board N16R8 MCU
// GPS -> gt-u7
// I2C display -> SSD1306
#include "Config.h"
#include "gps_config.h"
#include "ssd1306_config.h"
#include <Fonts/FreeMono9pt7b.h>

#include "WiFi.h"

IPAddress gateway;

WiFiServer server(12000);
WiFiClient client;

// connect to wifi network
void setup_wifi_client() {
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
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);

  gateway = WiFi.gatewayIP();
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println();
  Serial.print("gateway address: ");
  Serial.println(gateway.toString());

  server.begin();
  client = server.available();
}

/*
add this to GPS code and pass in server 
/ /so GPS can do a read and send
void wifi_client_send_gps(WiFiServer server, WiFiClient client) {

  long start = millis();

  while (!client &&  && (millis() - start) < 5000){
      if (Serial1.available() > 0){
      data[idx] = Serial1.read();
        if (data[idx] == LF) {
          data[idx-1] = 0;
//Serial.println(data);
          delay(10);
          server.println(data);
          idx = -1;
          }
      idx++;
      }
    }
}*/

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(9600);

  // Initialize the serial communication with the GPS module on UART2
  setup_gps();

  setup_SDD1306();

  setDisplayFont(&FreeMono9pt7b); 

  //setup_wifi_client();

#ifdef ESP32
  BaseType_t coreID = xPortGetCoreID();
  Serial.println("ESP32 GPS Receiver with TinyGPSPlus");
  Serial.print("Running on core ");
  Serial.println(coreID);

  /* 
  xTaskCreatePinnedToCore(
    myTask,        // Task function
    "MyTask",      // Name of task
    2048,          // Stack size (bytes)
    NULL,          // Parameter to pass to task
    1,             // Task priority
    NULL,          // Task handle
    0              // Core to run on (0 or 1)
  );
  */
#endif  
}

void loop() {

  loop_gps();

  displayInfo();

}

void displayInfo() {

  GPS_LOCATION myGPS = getGPSData();

  // clear the display
  clear_display();

  // Add a small delay to avoid flooding the serial monitor
  if (millis() > 5000 && !myGPS.updated) {
    Serial.println(F("No GPS data received. Possibly a wiring or antenna issue."));
    // You may want to reset the ESP32 here if no data is received for a long time
    write_display(2, 16, "No GPS\ndata!");
    show_display();
    delay(2500);
    return;
  }

  Serial.print("Location: ");
  if (myGPS.updated) {
    Serial.print(myGPS.latitude, 6);
    Serial.print(", ");
    Serial.print(myGPS.longitude, 6);
  } else {
    Serial.print("Invalid");
  }

  Serial.print(" Date: ");
  Serial.print(myGPS.gps_date);

  Serial.print(" Time: ");
  Serial.print(myGPS.gps_time);

  Serial.println();

  clear_display();
  char buffer[16];
  long degrees;
  long minutes;
  long minutesTemp;
  long seconds;

  // force 6 decimal places
  double latitude = myGPS.latitude > 0 ? floor(myGPS.latitude * 1000000) / 1000000 : ceil(myGPS.latitude * 1000000) / 1000000;
  double longitude = myGPS.longitude > 0 ? floor(myGPS.longitude * 1000000) / 1000000 : ceil(myGPS.longitude * 1000000) / 1000000;
  
  degrees = trunc(latitude);
  minutesTemp = (abs(latitude) - abs(degrees)) * 60;
  minutes = trunc(minutesTemp);seconds = (minutesTemp - trunc(minutesTemp)) * 100 * 60 / 100;
  seconds = trunc((minutesTemp - minutes) * 60);

  memset(buffer, '\0', 10);
  snprintf(buffer, sizeof(buffer), "%d:%d", degrees, minutes);

  write_display(2, 11, "Latitude:");
  write_display(100 - (strlen(buffer) * 12), 27, buffer);  

  degrees = trunc(longitude);
  minutesTemp = (abs(longitude) - abs(degrees)) * 60;
  minutes = trunc(minutesTemp);seconds = (minutesTemp - trunc(minutesTemp)) * 100 * 60 / 100;
  seconds = trunc((minutesTemp - minutes) * 60);

  memset(buffer, '\0', 10);
  snprintf(buffer, sizeof(buffer), "%d:%d", degrees, minutes);

  write_display(2, 41, "Longitude:");
  write_display(100 - (strlen(buffer) * 12), 61, buffer);  

  show_display();
  delay(10000);

  clear_display();
  write_display(2, 11, "Date:");
  write_display(8, 27, myGPS.gps_date);  
  write_display(2, 41, "Time:");
  write_display(30, 61, myGPS.gps_time);  

  show_display();
  delay(3500);
}
