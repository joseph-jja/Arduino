// designed for the ESP32 S3 dual core!
// ESP32 S3 -> Development Board N16R8 MCU
// GPS -> gt-u7
// I2C display -> SSD1306
#include "gps_config.h"
#include "sdd1306_config.h"

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(115200);

  // Initialize the serial communication with the GPS module on UART2
  setup_gps();

  setup_SDD1306();

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
  delay(1000);
}

void displayInfo() {

  GPS_LOCATION myGPS = getGPSData();

  // Add a small delay to avoid flooding the serial monitor
  if (millis() > 5000 && !myGPS.updated) {
    Serial.println(F("No GPS data received: check wiring or antenna."));
    // You may want to reset the ESP32 here if no data is received for a long time
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

  write_display(2, 2, "Latitude");
  write_number(20, 2, myGPS.latitude);  
  write_display(2, 12, "Longitude");
  write_number(20, 12, myGPS.longitude);  

  write_display(2, 32, "Date");
  write_number(20, 32, myGPS.latitude);  
  write_display(2, 42, "Time");
  write_number(20, 42, myGPS.longitude);  
}
