// -------------------------------------------------------------------------------------------------
// Pin map for OnStep MiniPCB-S3 Version 3.x (ESP32S)
#pragma once

#if defined(ESP32)

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
#if SERIAL_D_BAUD_DEFAULT != OFF
  #define SERIAL_D              Serial3
#endif

// Use the following settings for any TMC UART driver (TMC2209) that may be present
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define SERIAL_TMC_HARDWARE_UART
  #define SERIAL_TMC            Serial1          // Use a single hardware serial port to up to four drivers
  #define SERIAL_TMC_BAUD       460800           // Baud rate
  #define SERIAL_TMC_RX         11               // Recieving data
  #ifndef SERIAL_TMC_TX
    #define SERIAL_TMC_TX       19                // Transmit data
  #endif
  #define SERIAL_TMC_ADDRESS_MAP(x) ((x==4)?2 : x) // Axis1(0) is 0, Axis2(1) is 1, Axis3(2) is 2, Axis4(3) is 3, Axis5(4) is 2
#endif


// The multi-purpose pins (Aux3..Aux8 can be analog pwm/dac if supported)
#define AUX0_PIN                18               // Status LED
#define AUX1_PIN                40               // ESP32 S3 Mini
#define AUX2_PIN                11                // ESP8266 RST, SPI MISO/Fault
#define AUX3_PIN                39                // Limit SW, Home SW
//#define AUX4_PIN                47               // Reticle LED, Home SW
// lots of free pins to choose from
//#define AUX5_PIN              DAC_PIN(A14)     // true analog output

// Misc. pins
// TODO update
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          0               // Default Pin for OneWire bus
#endif

#define ADDON_GPIO0_PIN       AUX1_PIN         // ESP8266 GPIO0 or SPI MISO/Fault
#define ADDON_RESET_PIN       AUX2_PIN         // ESP8266 RST or SPI MISO/Fault
// The PEC index sense is a logic level input, resets the PEC index on rising edge then waits for 60 seconds before allowing another reset
#define PEC_SENSE_PIN           48               // PEC Sense, analog or digital

// The status LED is a two wire jumper with a 10k resistor in series to limit the current to the LED
#define STATUS_LED_PIN          AUX0_PIN         // Default LED Cathode (-)
#define MOUNT_LED_PIN           AUX0_PIN         // Default LED Cathode (-)
#ifndef RETICLE_LED_PIN 
  #define RETICLE_LED_PIN       AUX4_PIN         // Default LED Cathode (-)
#endif

// For a piezo buzzer
#ifndef STATUS_BUZZER_PIN 
  #define STATUS_BUZZER_PIN     3               // Tone
#endif

// The PPS pin is a 3.3V logic input, OnStep measures time between rising edges and adjusts the internal sidereal clock frequency
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         41               // PPS time source, GPS for example
#endif

#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX3_PIN         // The limit switch sense is a logic level input normally pull high (2k resistor,) shorted to ground it stops gotos/tracking
#endif

// hint that the driver mode pins are dedicated (not shared SPI bus except possibly MISO)
#define DEDICATED_MODE_PINS

// Axis1 RA/Azm step/dir driver
#define AXIS1_ENABLE_PIN    4
#define AXIS1_M0_PIN            OFF              // hardwired for TMC UART address 0
#define AXIS1_M1_PIN            OFF              // hardwired for TMC UART address 0
#define AXIS1_M2_PIN            OFF              // UART TX
#define AXIS1_M3_PIN            OFF              // UART RX
#define AXIS1_STEP_PIN      5
#define AXIS1_DIR_PIN       6
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN
#endif

// Axis2 Dec/Alt step/dir driver
#define AXIS2_ENABLE_PIN    4
#define AXIS2_M0_PIN            OFF              // hardwired for TMC UART address 1
#define AXIS2_M1_PIN            OFF              // hardwired for TMC UART address 1
#define AXIS2_M2_PIN            OFF              // N/C
#define AXIS2_M3_PIN            OFF              // UART RX
#define AXIS2_STEP_PIN      3
#define AXIS2_DIR_PIN       10
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN
#endif

// ST4 interface
#define ST4_RA_W_PIN        36      // ST4 RA- West
#define ST4_DEC_S_PIN       35      // ST4 DE- South
#define ST4_DEC_N_PIN       37      // ST4 DE+ North
#define ST4_RA_E_PIN        38     // ST4 RA+ East

#else
#error "Wrong processor for this configuration!"

#endif
