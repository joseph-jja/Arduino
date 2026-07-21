// Matter Manager
#include <Arduino.h>
#include <Matter.h>
#if !CONFIG_ENABLE_CHIPOBLE
// if the device can be commissioned using BLE, WiFi is not used - save flash space
#include <WiFi.h>
#endif
#include "esp_sleep.h"

#define ESP32_S3_ENABLED true

// List of Matter Endpoints for this Node
// Matter Contact Sensor Endpoint
MatterContactSensor ContactSensor;

// Use an RTC GPIOus

#define SENSOR_PIN 5 

// CONFIG_ENABLE_CHIPOBLE is enabled when BLE is used to commission the Matter Network
#if !CONFIG_ENABLE_CHIPOBLE
// WiFi is manually set and started
const char *ssid = "your-ssid";          // Change this to your WiFi SSID
const char *password = "your-password";  // Change this to your WiFi password
#endif
 
// LED will be used to indicate the Contact Sensor state
// set your board RGB LED pin here
#ifdef RGB_BUILTIN
const uint8_t ledPin = RGB_BUILTIN;
#else
const uint8_t ledPin = 2;  // Set your pin here if your board has not defined LED_BUILTIN
#warning "Do not forget to set the RGB LED pin"
#endif

const uint8_t buttonPin = BOOT_PIN;  // Set your pin here. Using BOOT Button.

// Button control
uint32_t button_time_stamp = 0;                // debouncing control
bool button_state = false;                     // false = released | true = pressed
const uint32_t debouceTime = 250;              // button debouncing time (ms)
const uint32_t decommissioningTimeout = 5000;  // keep the button pressed for 5s, or longer, to decommission

// look here for code example https://docs.espressif.com/projects/arduino-esp32/en/latest/matter/ep_contact_sensor.html
void setup() {
    Serial.begin(115200);

    // 1. Initialize Matter
    // Configure your Contact Sensor endpoint here...
    
    // 2. Read Switch
    pinMode(SENSOR_PIN, INPUT_PULLUP);
    bool isOpen = digitalRead(SENSOR_PIN);
    
    // 3. Update Matter Attribute
    // Update the 'ContactStatus' attribute in the Matter cluster
    
    // 4. Configure Sleep
    // Wake up when the pin level changes (e.g., HIGH to LOW)
    #ifdef ESP32_S3_ENABLED 
    esp_sleep_enable_ext0_wakeup((gpio_num_t)SENSOR_PIN, ESP_EXT1_WAKEUP_ANY_LOW);
    #else
    esp_deep_sleep_enable_gpio_wakeup(1 << SENSOR_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
    #endif
    
    // 5. Sleep
    Serial.println("Going to sleep...");
    esp_deep_sleep_start();
}

void loop() {
    // This code will not be reached if you use deep sleep
}
