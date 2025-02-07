#include <stdlib.h>
#include <string.h>
#include <math.h>

// Import required libraries for esp8266
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

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
ESP8266WebServer server(80);

// accel and gyro code from 
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;

long currentHour, 
   currentMinute,
   latitude,
   longitude;

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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Route for root / web page
  //Serial.println(index_html);
  server.on("/", []() {
    Serial.println("Request for home page");
    server.send(200, "text/html", index_html);
  });
  /*server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });*/

  // Start server
  //server.begin();

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  
  Wire.write(0x00);    
  Wire.endTransmission(true);

  // talk to the accel
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);  
  Wire.write(0x10);  
  Wire.endTransmission(true);
  // talk to the gps
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);  
  Wire.write(0x10);  
  Wire.endTransmission(true);
  delay(20);
  // Call this function if you need to get the IMU error values for your module
  calculate_IMU_error();

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Pin enabled");
  digitalWrite(LED_BUILTIN, LOW);
}

void blink_pin(int sleep_time) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(sleep_time);
    digitalWrite(LED_BUILTIN, LOW);
}

void get_gps_info() {

  long avail = ss.available();

  char buff[255];

  bool isLatLongValid = false;
  bool gotGPSTime = false;

  while (avail > 0) {
    //Serial.println("Trying to get GPS data.");
    if (gps.encode(ss.read())) {
        blink_pin(100);
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
  delay(500);
  long gps = (isLatLongValid ? 100 : 0) + (gotGPSTime ? 5 : 0);

}

void loop() {

  char buff[255];

  blink_pin(1000);  
  
  get_gps_info();
  delay(500);

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value  

  // Calculating Roll and Pitch from the accelerometer data
  // AccErrorX ~(0.58) See the      calculate_IMU_error()custom function for more details
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; 
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)

  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  // Read 4 registers total, each axis value is stored in 2 registers
  Wire.requestFrom(MPU, 6, true); 
  // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; 
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  // Currently the raw values are in degrees per seconds, deg/s,
  // so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AccX);
  Serial.print(" | Y = "); Serial.print(AccY);
  Serial.print(" | Z = ");  Serial.println(AccZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X  = "); Serial.print(GyroX);
  Serial.print(" | Y = "); Serial.print(GyroY);
  Serial.print(" | Z = "); Serial.println(GyroZ);

  // Print the values on the serial monitor
  Serial.print(roll);
  Serial.print("/");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);  delay(333);
}

void calculate_IMU_error() {

  int c = 0;
  
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}
