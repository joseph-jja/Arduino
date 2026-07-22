// Matter Manager
#include <Arduino.h>
#include <Matter.h>

#undef CONFIG_ENABLE_CHIPOBLE
#define ESP32_S3_ENABLED
#define ENABLE_MATTER_CODE
#define ENABLE_SLEEP_CODE

#if !CONFIG_ENABLE_CHIPOBLE
// if the device can be commissioned using BLE, WiFi is not used - save flash space
#include <WiFi.h>
#endif
#include "esp_sleep.h"

#include "config.h"

// List of Matter Endpoints for this Node
// Matter Contact Sensor Endpoint
MatterContactSensor ContactSensor;

// Use an RTC GPIO
#define SENSOR_PIN 4
const gpio_num_t WAKEUP_PIN = (gpio_num_t)SENSOR_PIN; // Fixed: Passed raw pin number for ext0 instead of bitshift

// CONFIG_ENABLE_CHIPOBLE is enabled when BLE is used to commission the Matter Network
#if !CONFIG_ENABLE_CHIPOBLE
// WiFi is manually set and started
const char *ssid = WIFI_SSID;          // Change this to your WiFi SSID
const char *password = WIFI_PASSWORD;  // Change this to your WiFi password
#endif

// LED will be used to indicate the Contact Sensor state
// set your board RGB LED pin here
#ifdef RGB_BUILTIN
const uint8_t ledPin = RGB_BUILTIN;
#else
const uint8_t ledPin = 48;  // Set your pin here if your board has not defined LED_BUILTIN
#warning "Do not forget to set the RGB LED pin"
#endif

const uint8_t buttonPin = SENSOR_PIN;  // Set your pin here. Using BOOT Button.

// Button control
uint32_t button_time_stamp = 0;                // debouncing control
bool button_state = true;                      // false = released | true = pressed
const uint32_t debouceTime = 250;              // button debouncing time (ms)
const uint32_t decommissioningTimeout = 5000;  // keep the button pressed for 5s, or longer, to decommission

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  // Read current state upon boot (HIGH = Open, LOW = Closed for NC switch)
  bool openState = digitalRead(buttonPin);
  digitalWrite(ledPin, !openState);

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1: Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP: Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

// look here for code example https://docs.espressif.com/projects/arduino-esp32/en/latest/matter/ep_contact_sensor.html
void setup() {
  Serial.begin(115200);
  delay(1000);

  // 1. Initialize Matter
  // Configure your Contact Sensor endpoint here...
  ContactSensor = MatterContactSensor();

  // 2. Read Switch
  pinMode(buttonPin, INPUT_PULLUP);

  // led
  pinMode(ledPin, OUTPUT);

  // CONFIG_ENABLE_CHIPOBLE is enabled when BLE is used to commission the Matter Network
#if !CONFIG_ENABLE_CHIPOBLE
  // Manually connect to WiFi
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi connected!");
#endif

  // 3. Update Matter Attribute
  // Update the 'ContactStatus' attribute in the Matter cluster
  ContactSensor.begin();
  
  // Read current state upon boot (HIGH = Open, LOW = Closed for NC switch)
  bool initialOpenState = digitalRead(buttonPin);
  ContactSensor.setContact(initialOpenState);
  digitalWrite(ledPin, !initialOpenState);

  Serial.print("Initial state: ");
  Serial.println(initialOpenState);

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  // Matter beginning - Last step, after all EndPoints are initialized
  Matter.begin();

#ifdef ENABLE_MATTER_CODE
  // Check Matter Accessory Commissioning state, which may change during execution of loop()
  if (!Matter.isDeviceCommissioned()) {
    Serial.println("");
    Serial.println("Matter Node is not commissioned yet.");
    Serial.println("Initiate the device discovery in your Matter environment.");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\r\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\r\n", Matter.getOnboardingQRCodeUrl().c_str());
    // waits for Matter Contact Sensor Commissioning.
    uint32_t timeCount = 0;
    while (!Matter.isDeviceCommissioned()) {
      delay(100);
      if ((timeCount++ % 50) == 0) {  // 50*100ms = 5 sec
        Serial.println("Matter Node not commissioned yet. Waiting for commissioning.");
      }
    }
    Serial.println("Matter Node is commissioned and connected to the network. Ready for use.");
  }
#endif // Fixed typo here (was #indif)

#ifdef ENABLE_SLEEP_CODE
// 4. Configure Sleep
// Wake up when the pin level changes (e.g., HIGH to LOW)
#ifdef ESP32_S3_ENABLED
  // EXT0 configuration for ESP32-S3: 
  // WAKEUP_PIN, level (1 = wake up when pin goes HIGH, i.e., door opens)
  esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);
#else
  esp_deep_sleep_enable_gpio_wakeup((1ULL << SENSOR_PIN), ESP_GPIO_WAKEUP_GPIO_HIGH);
#endif

  // 5. Sleep
  Serial.println("Going to sleep...");
  Serial.flush();
  esp_deep_sleep_start();
#endif
}

void loop() {
#ifdef ENABLE_SLEEP_CODE
  // This code will not be reached if you use deep sleep
#else
  bool isOpen = digitalRead(buttonPin);
  ContactSensor.setContact(isOpen);
  Serial.print("Testing the sketch state: ");
  Serial.print(isOpen);
  Serial.print(" / ");
  Serial.println(ContactSensor.getContact());
  digitalWrite(ledPin, !isOpen);  // LED ON
  delay(1000); // Fixed standard Arduino delay instead of sleep(1)
#endif
}
