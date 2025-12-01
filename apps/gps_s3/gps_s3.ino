// designed for the ESP32 S3 dual core!
// ESP32 S3 -> Development Board N16R8 MCU
// GPS -> gt-u7
// I2C display -> SSD1306
#include "gps_config.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  // Initialize the serial monitor for debugging
  Serial.begin(115200);

  // Initialize the serial communication with the GPS module on UART2
  setup_gps();

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

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();

  // Clear the buffer.
  display.clearDisplay();
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

  // setup text size
  display.setTextSize(2);

  // text color
  display.setTextColor(WHITE);

  /* 
  // write data at positions
  display.setCursor(x, y);

  memset(buff, '\0', sizeof(buff));
  // fill buffer
  display.println(buff);

  display.display();
  */
}
