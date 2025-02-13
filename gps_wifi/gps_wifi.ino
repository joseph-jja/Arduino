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
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
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

// MPU6050 
Adafruit_MPU6050 mpu;

float AccX = 0.0,
    AccY = 0.0,
    AccZ = 0.0;
float GyroX = 0.0,
    GyroY = 0.0,
    GyroZ = 0.0;

float temperatureC = 0.0,
    temperatureF = 0.0;

float currentHour = 0, 
   currentMinute = 0,
   latitude = 0,
   longitude = 0;

void setupMPU6050() {
    Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  } 
}

void setup() {
  
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
  server.on("/update", []() {
    char temp[512];
    memset(temp, '\0', sizeof(temp));
    sprintf(temp, "{ \"degC\": %2.2f, \"degF\": %2.2f, \"latitude\": %3.2f, \"longitude\": %3.2f, \"time\": \"%2.0f:%2.0f\", \"Acc\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f }, \"Gyro\": { \"X\": %3.2f, \"Y\": %3.2f , \"Z\": %3.2f } }", 
        temperatureC, temperatureF,
        latitude, longitude,
        currentHour, currentMinute,
        AccX, AccY, AccZ,
        GyroX, GyroY, GyroZ);
    Serial.print("Update request ");
    Serial.println(temp);
    server.send(200, "text/html", temp);
  });

  // Start server
  server.begin();

  // Try to initialize MPU6050!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  setupMPU6050();

   // uncomment this to calibrate offsets
  //calculate_offsets();

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

  while (avail > 0) {
    //Serial.println("Trying to get GPS data.");
    if (gps.encode(ss.read())) {
        blink_pin(100);
        Serial.println("GPS data obtained");
        //writeString(100, 'B');
        
        //memset(&buff, '\0', sizeof(buff));
        //sprintf(buff, "Latitude: %6d \t Longitude: %6d ", gps.location.lat(), gps.location.lng()); 
        //Serial.println(buff);
        if (gps.location.isValid()) {
          //writeString(ll, 'C');
          // set these
          latitude = gps.location.lat();
          longitude = gps.location.lng();
          Serial.print("Got valid latitude and longitude");
          Serial.print(latitude, 6);
          Serial.print(longitude, 6);
          Serial.println(" ");
          delay(1000);
        }
        if (gps.time.isValid()) {
          currentHour = gps.time.hour();
          currentMinute = gps.time.minute();
          Serial.print("Got valid time ");
          if (gps.time.hour() < 10) Serial.print(F("0"));
          Serial.print(currentHour);
          Serial.print(F(":"));
          if (gps.time.minute() < 10) Serial.print(F("0"));
          Serial.print(currentMinute);
          Serial.println(" ");
        }
    }
    avail = ss.available();
  }
  delay(500);

}

void getAccelerometerData() {

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  AccX = a.acceleration.x;
  AccY = a.acceleration.y;
  AccZ = a.acceleration.z - AccelOffsetZ;
  GyroX = g.gyro.x - GyroOffsetX;
  GyroY = g.gyro.y - GyroOffsetY;
  GyroZ = g.gyro.z - GyroOffsetZ;
  temperatureC = temp.temperature;
  temperatureF = (C2F_MULTIPLIER * temperatureC) + C2F_ADDITION;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(AccX);
  Serial.print(" ");
  Serial.print(AccX - AccelOffsetX);
  Serial.print(" ");
  Serial.print(", Y: ");
  Serial.print(AccY);
  Serial.print(" ");
  Serial.print(AccY - AccelOffsetY);
  Serial.print(", Z: ");
  Serial.print(AccZ);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(GyroX);
  Serial.print(", Y: ");
  Serial.print(GyroY);
  Serial.print(", Z: ");
  Serial.print(GyroZ);
  Serial.println(" rad/s");

  Serial.print("Chip Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" degC  ");
  Serial.print(temperatureF);
  Serial.println(" degF");

  Serial.println("");
}

void loop() {

  char buff[255];

  blink_pin(1000);  
  
  get_gps_info();
  delay(100);

  getAccelerometerData();
  delay(500);
    
  server.handleClient();
  MDNS.update();
}
