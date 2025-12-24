// designed for the ESP32 S3 dual core!
// ESP32 S3 -> Development Board N16R8 MCU
// GPS -> gt-u7
// I2C display -> SSD1306
#include "gps_config.h"
#include "sdd1306_config.h"

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(9600);

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

  // clear the display
  clear_display();

  // Add a small delay to avoid flooding the serial monitor
  if (millis() > 5000 && !myGPS.updated) {
    Serial.println(F("No GPS data received: check wiring or antenna."));
    // You may want to reset the ESP32 here if no data is received for a long time
    write_display(2, 2, "No GPS data yet!");
    show_display();
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

  float latitude = myGPS.latitude;
  float longitude = myGPS.longitude;

  char buffer[10];
  memset(buffer, '\0', 10);
  sprintf(buffer, "%d:%2d", floor(latitude), ((latitude - floor(latitude)) / 60));

  write_display(2, 2, "Latitude");
  write_display(64, 2, buffer);  

  memset(buffer, '\0', 10);
  sprintf(buffer, "%d:%2d", floor(longitude), ((longitude - floor(longitude)) / 60));

  write_display(2, 12, "Longitude");
  write_number(64, 12, myGPS.longitude);  

  write_display(2, 32, "Date");
  write_display(64, 32, myGPS.gps_date);  
  write_display(2, 42, "Time");
  write_display(64, 42, myGPS.gps_time);  

  show_display();
}
