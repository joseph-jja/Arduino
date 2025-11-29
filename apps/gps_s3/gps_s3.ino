// designed for the ESP32 S3 dual core!
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// Define the ESP32-S3 pins for UART2 (adjust if your hardware is different)
// Pin 34 is for TXD2, and Pin 36 is for RXD2 in this example.
#define GPS_RXD2 17
#define GPS_TXD2 18

// Define the GPS baud rate (e.g., 9600)
#define GPS_BAUD 9600

// Create a HardwareSerial instance for UART2
HardwareSerial gpsSerial(2);

// The TinyGPSPlus object
TinyGPSPlus gps;



void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(115200);

  // Initialize the serial communication with the GPS module on UART2
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RXD2, GPS_TXD2);

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
}

void loop() {
  // Read data from the GPS module and encode it with TinyGPSPlus
  while (gpsSerial.available() > 0) {
    Serial.println("Data available!");
    if (gps.encode(gpsSerial.read())) {
      // If a new sentence is encoded, display the information
      displayInfo();
    } else{
      Serial.println("Uggos");
    }
  }

  // Add a small delay to avoid flooding the serial monitor
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS data received: check wiring or antenna."));
    // You may want to reset the ESP32 here if no data is received for a long time
  }
}

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("Invalid"));
  }

  Serial.print(F(" Date: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("Invalid"));
  }

  Serial.print(F(" Time: "));
  if (gps.time.isValid()) {
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    Serial.print(gps.time.second());
  } else {
    Serial.print(F("Invalid"));
  }

  Serial.println();
}

