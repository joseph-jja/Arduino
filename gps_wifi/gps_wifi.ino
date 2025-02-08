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

// accel and gyro code from 
// https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
float AccX = 0.0,
    AccY = 0.0,
    AccZ = 0.0;
float GyroX = 0.0,
    GyroY = 0.0,
    GyroZ = 0.0;

float temperatureC = 0.0,
    temperatureF = 0.0;

long currentHour = 0, 
   currentMinute = 0,
   latitude = 0,
   longitude = 0;

void setupMPU6050() {
    Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
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


  //setup motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);
}

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
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    char temp[1248];
    memset(temp, '\0', sizeof(temp));    
    /*sprintf(temp, index_html, temperatureC, temperatureF,
        latitude, longitude,
        currentHour, currentMinute, 
        AccX, AccY, AccZ,
        GyroX, GyroY, GyroZ);*/
    request->send_P(200, "text/plain", "{}");
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
          currentHour = gps.time.hour();
          currentMinute = gps.time.minute();
        }
    }
    avail = ss.available();
  }
  delay(500);
  long gps = (isLatLongValid ? 100 : 0) + (gotGPSTime ? 5 : 0);

}

void getAccelerometerData() {

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  AccX = a.acceleration.x;
  AccY = a.acceleration.y;
  AccZ = a.acceleration.z; // earths gravity is default
  GyroX = g.gyro.x;
  GyroY = g.gyro.y;
  GyroZ = g.gyro.z;
  temperatureC = temp.temperature;
  temperatureF = (C2F_MULTIPLIER * temperatureC) + C2F_ADDITION;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(AccX);
  Serial.print(", Y: ");
  Serial.print(AccY);
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

  Serial.print("Temperature: ");
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
