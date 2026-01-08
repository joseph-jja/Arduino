// -------------------------------------------------------------------------------------------------
// Pin map for OnStep MiniPCB-S3 Version 3.x (ESP32S)
#pragma once

#if defined(ESP32)

/*
// TMC2130 SPI Bus Definition (Shared)
#define TMC_MISO_PIN 13
#define TMC_MOSI_PIN 12
#define TMC_SCK_PIN 11

// Axis1 RA/Azm step/dir driver
// AXIS1 (RA) Configuration
#define AXIS1_ENABLE_PIN    16
#define AXIS1_M0_PIN        12      // Shared MOSI
#define AXIS1_M1_PIN        11      // Shared SCK
#define AXIS1_M2_PIN        10      // Unique CSN
#define AXIS1_M3_PIN        13      // Shared MISO
#define AXIS1_STEP_PIN      8
#define AXIS1_DIR_PIN       6
#define AXIS1_FAULT_PIN     AXIS1_M3_PIN      // Optional fault pin

// AXIS2 (DEC) Configuration
#define AXIS2_ENABLE_PIN    15
#define AXIS2_M0_PIN        12      // Shared MOSI
#define AXIS2_M1_PIN        11      // Shared SCK
#define AXIS2_M2_PIN        9       // Unique CSN
#define AXIS2_M3_PIN        13      // Shared MISO
#define AXIS2_STEP_PIN      7
#define AXIS2_DIR_PIN       5
#define AXIS2_FAULT_PIN     AXIS2_M3_PIN      // Optional fault pin
*/

// Serial0: USB serial adapter
// Serial1: RX Pin U0, TX Pin U0
// Serial2: RX1 Pin GPIO6, TX1 Pin GPIO5

#if SERIAL_A_BAUD_DEFAULT != OFF
  #define SERIAL_A              Serial
#endif
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B              Serial1
#endif
#if SERIAL_C_BAUD_DEFAULT != OFF
  #define SERIAL_C              Serial2
#endif
#if defined(USB_DUAL_SERIAL) || defined(USB_TRIPLE_SERIAL)
  #define SERIAL_D              SerialUSB1
  #define SERIAL_D_BAUD_DEFAULT 9600
#endif
#if defined(USB_TRIPLE_SERIAL)
  #define SERIAL_E              SerialUSB2
  #define SERIAL_E_BAUD_DEFAULT 9600
#endif

// The multi-purpose pins (Aux3..Aux8 can be analog pwm/dac if supported)
#define AUX0_PIN                48               // Status LED
#define AUX1_PIN                13               // ESP8266 GPIO0, SPI MISO/Fault
#define AUX2_PIN                RST                // ESP8266 RST, SPI MISO/Fault
#define AUX3_PIN                18                // Limit SW, Home SW
#define AUX4_PIN                10               // Reticle LED, Home SW
// lots of free pins to choose from
//#define AUX5_PIN              DAC_PIN(A14)     // true analog output

// Misc. pins
// TODO update
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          21               // Default Pin for OneWire bus
#endif

#define ADDON_GPIO0_PIN       AUX1_PIN         // ESP8266 GPIO0 or SPI MISO/Fault
#define ADDON_RESET_PIN       AUX2_PIN         // ESP8266 RST or SPI MISO/Fault
// The PEC index sense is a logic level input, resets the PEC index on rising edge then waits for 60 seconds before allowing another reset
#define PEC_SENSE_PIN           47               // PEC Sense, analog or digital

// The status LED is a two wire jumper with a 10k resistor in series to limit the current to the LED
#define STATUS_LED_PIN          AUX0_PIN         // Default LED Cathode (-)
#define MOUNT_LED_PIN           AUX0_PIN         // Default LED Cathode (-)
#ifndef RETICLE_LED_PIN 
  #define RETICLE_LED_PIN       AUX4_PIN         // Default LED Cathode (-)
#endif

// For a piezo buzzer
#ifndef STATUS_BUZZER_PIN 
  #define STATUS_BUZZER_PIN     45               // Tone
#endif

// The PPS pin is a 3.3V logic input, OnStep measures time between rising edges and adjusts the internal sidereal clock frequency
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         21               // PPS time source, GPS for example
#endif

#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX3_PIN         // The limit switch sense is a logic level input normally pull high (2k resistor,) shorted to ground it stops gotos/tracking
#endif

// hint that the driver mode pins are dedicated (not shared SPI bus except possibly MISO)
#define DEDICATED_MODE_PINS

// Axis1 RA/Azm step/dir driver
#define AXIS1_ENABLE_PIN    16
#define AXIS1_M0_PIN        12      // Shared MOSI
#define AXIS1_M1_PIN        11      // Shared SCK
#define AXIS1_M2_PIN        10      // Unique CSN
#define AXIS1_M3_PIN        13      // Shared MISO
#define AXIS1_STEP_PIN      8
#define AXIS1_DIR_PIN       6
#define AXIS1_FAULT_PIN     AXIS1_M3_PIN      // Optional fault pin
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN
#endif

// Axis2 Dec/Alt step/dir driver
#define AXIS2_ENABLE_PIN    15
#define AXIS2_M0_PIN        12      // Shared MOSI
#define AXIS2_M1_PIN        11      // Shared SCK
#define AXIS2_M2_PIN        9       // Unique CSN
#define AXIS2_M3_PIN        13      // Shared MISO
#define AXIS2_STEP_PIN      7
#define AXIS2_DIR_PIN       5
#define AXIS2_FAULT_PIN     AXIS2_M3_PIN      // Optional fault pin
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN
#endif

// For rotator stepper driver
#define AXIS3_ENABLE_PIN        OFF
#define AXIS3_M0_PIN            OFF              // SPI MOSI
#define AXIS3_M1_PIN            OFF              // SPI SCK
#define AXIS3_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS3_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS3_STEP_PIN          30
#define AXIS3_DIR_PIN           33

// For focuser1 stepper driver
#define AXIS4_ENABLE_PIN        OFF
#define AXIS4_M0_PIN            OFF              // SPI MOSI
#define AXIS4_M1_PIN            OFF              // SPI SCK
#define AXIS4_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS4_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS4_STEP_PIN          31
#define AXIS4_DIR_PIN           32

// For focuser2 stepper driver
#define AXIS5_ENABLE_PIN        OFF
#define AXIS5_M0_PIN            OFF              // SPI MOSI
#define AXIS5_M1_PIN            OFF              // SPI SCK
#define AXIS5_M2_PIN            OFF              // SPI CS (UART TX)
#define AXIS5_M3_PIN            OFF              // SPI MISO (UART RX)
#define AXIS5_STEP_PIN          30
#define AXIS5_DIR_PIN           33

// ST4 interface
#define ST4_RA_W_PIN            2               // ST4 RA- West
#define ST4_DEC_S_PIN           5               // ST4 DE- South
#define ST4_DEC_N_PIN           4               // ST4 DE+ North
#define ST4_RA_E_PIN            1               // ST4 RA+ East

#else
#error "Wrong processor for this configuration!"

#endif
