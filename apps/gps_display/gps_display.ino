// designed for the ESP32 S3 dual core!
// ESP32 S3 -> Development Board N16R8 MCU
// GPS -> gt-u7
// I2C display -> SSD1306
#include "gps_config.h"
#include "sdd1306_config.h"
#include <Fonts/FreeMono9pt7b.h>

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(9600);

  // Initialize the serial communication with the GPS module on UART2
  setup_gps();

  setup_SDD1306();

  setDisplayFont(&FreeMono9pt7b); 

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
    write_display(2, 16, "No GPS\ndata!");
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

  clear_display();
  char buffer[10];
  long intPart;
  long floatPart;

  float latitude = floor(myGPS.latitude * 100) / 100;
  float longitude = floor(myGPS.longitude * 100) / 100;

  intPart = floor(latitude);
  floatPart = abs(floor(((abs(latitude) - abs(intPart)) * 100) * 60 / 100));

  memset(buffer, '\0', 10);
  sprintf(buffer, "%d:%d", intPart, floatPart);

  write_display(2, 11, "Latitude:");
  write_display(100 - (strlen(buffer) * 12), 27, buffer);  

  intPart = floor(longitude);
  floatPart = abs(floor(((abs(longitude) - abs(intPart)) * 100) * 60 / 100));

  memset(buffer, '\0', 10);
  sprintf(buffer, "%d:%d", intPart, floatPart);

  write_display(2, 41, "Longitude:");
  write_display(100 - (strlen(buffer) * 12), 61, buffer);  

  show_display();
  delay(5000);

  clear_display();
  write_display(2, 11, "Date:");
  write_display(8, 27, myGPS.gps_date);  
  write_display(2, 41, "Time:");
  write_display(30, 61, myGPS.gps_time);  

  show_display();
  delay(2500);
}
