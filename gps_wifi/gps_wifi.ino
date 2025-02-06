#include <stdlib.h>
#include <string.h>
#include <math.h>

// Import required libraries for esp8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebSrv.h>

// accelerometer
#include <Wire.h>

// gps
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// configuration
#include "Config.h"

// separate file for this lovely HTML 
#include "static_html.h"

IPAddress local_IP(192,168,25,1);
IPAddress gateway(192,168,25,1);
IPAddress subnet(255,255,255,0);

// The TinyGPS object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Create AsyncWebServer object on port 80
//AsyncWebServer server(80);

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup()
{
  
  Serial.begin(SERIAL_BAUD);
  delay(100);
  Serial.println("Application setup!");

  Serial.print("GPS library ");
  Serial.println(TinyGPSPlus::libraryVersion());
  ss.begin(GPSBaud);
  
  Serial.print("Setting up Access Point");
  WiFi.softAPConfig(local_IP, gateway, subnet); 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  //Serial.println(index_html);
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();*/

  /*Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);*/

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Pin enabled");
  digitalWrite(LED_BUILTIN, LOW);
}


long currentHour, 
   currentMinute,
   latitude,
   longitude;

signed long tzOffset = -7;

bool gotGPSTime = false;
bool isLatLongValid = false;

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{

  char buff[255];

  blink_pin(1000);  
  
  long avail = ss.available();
  /*memset(&buff, '\0', sizeof(buff));
  sprintf(buff, "Start %d ", avail);
  Serial.println(buff);
  writeString(avail, 'A');*/

  isLatLongValid = false;
  gotGPSTime = false;

  while (avail > 0) {
    blink_pin(100);
    Serial.println("Trying to get GPS data obtained");
    if (gps.encode(ss.read())) {
        Serial.println("GPS data obtained");
        //writeString(100, 'B');
        
        memset(&buff, '\0', sizeof(buff));
        sprintf(buff, "Latitude: %6d \t Longitude: %6d ", gps.location.lat(), gps.location.lng()); 
        Serial.println(buff);
        if (gps.location.isValid()) {
          long ll = floor(gps.location.lat() + gps.location.lng());
          //writeString(ll, 'C');
          Serial.print(gps.location.lat(), 6);
          Serial.print(gps.location.lng(), 6);
          Serial.println(" ");
          isLatLongValid = true;
          delay(1000);
        }
        if (gps.time.isValid()) {
          gotGPSTime = true;
          if (gps.time.hour() < 10) Serial.print(F("0"));
          Serial.print(gps.time.hour());
          Serial.print(F(":"));
          if (gps.time.minute() < 10) Serial.print(F("0"));
          Serial.print(gps.time.minute());
          //Serial.print(F(":"));
          /*if (gps.time.second() < 10) Serial.print(F("0"));
          Serial.print(gps.time.second());
          Serial.print(F("."));
          if (gps.time.centisecond() < 10) Serial.print(F("0"));
          Serial.print(gps.time.centisecond());*/
          Serial.println(" ");
          long hour = gps.time.hour();
          if (!isLatLongValid) {
            hour += tzOffset;
          }
          long time = (hour * 100) + gps.time.minute();

          latitude = gps.location.lat();
          longitude = gps.location.lng();
        }
    }
    avail = ss.available();
  }
  delay(1000);
  long gps = (isLatLongValid ? 100 : 0) + (gotGPSTime ? 5 : 0);
  delay(500);

  /*Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  /*int bytesRead = Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = ");  Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X  = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");*/
  delay(333);
}
