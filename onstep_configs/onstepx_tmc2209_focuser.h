/* ---------------------------------------------------------------------------------------------------------------------------------
 * Configuration for OnStepX
 *
 *          For more information on setting OnStep up see http://www.stellarjourney.com/index.php?r=site/equipment_onstep
 *                      and join the OnStep Groups.io at https://groups.io/g/onstep
 *
 *           *** Read the compiler warnings and errors, they are there to help guard against invalid configurations ***
 *
 * ---------------------------------------------------------------------------------------------------------------------------------
 * ADJUST THE FOLLOWING TO CONFIGURE YOUR CONTROLLER FEATURES ----------------------------------------------------------------------
 * <-Req'd = always must set, <-Often = usually must set, Option = optional, Adjust = adjust as req'd, Infreq = infrequently changed
*/
//      Parameter Name              Value   Default  Notes                                                                      Hint

// =================================================================================================================================
// CONTROLLER ======================================================================================================================

#define CONFIG_NAME       "xxxx"
//#define AP_SSID           "xxxx"         // set WiFi SSID name
//#define MDNS_NAME         "xxxx"         // set mDNS name
//#define SERIAL_RADIO WIFI_ACCESS_POINT
//#define PASSWORD_DEFAULT    "xxxx"  
//#define AP_PASSWORD         "xxxx"

#define STA_AP_FALLBACK         false
#define STA_ENABLED            true
#define STA_AUTO_RECONNECT            true
#define STA_DHCP_ENABLED          true
#define STA_SSID           "ssid to be client"         // set WiFi SSID name
#define SERIAL_RADIO       WIFI_STATION
#define PASSWORD_DEFAULT    "password for focuser"  
#define STA_PASSWORD         "ssid for ap"

#define FOCUSER_TEMPERATURE_PIN         2         // ESP32-S2 GPIO2 ADC1
#define THERMISTOR1_TNOM               25         // Nominal temperature (Celsius)
#define THERMISTOR1_RNOM            10000         // Nominal resistance (Ohms) at nominal temperature
#define THERMISTOR1_BETA             3950         // Beta coefficient
#define THERMISTOR1_RSERIES         33000         // Series resistor value (Ohms)
#define THERMISTOR_TEMPERATURE_MINIMUM -12.0

#define SERIAL_A                   Serial

// Use the following settings for any TMC UART driver (TMC2209) that may be present
#define DRIVER_TMC_STEPPER                        // use TMCStepper library
#define SERIAL_TMC_HARDWARE_UART
#define SERIAL_TMC                Serial1         // Use a single hardware serial port to up to four drivers
#define SERIAL_TMC_BAUD            460800         // Baud rate
#define SERIAL_TMC_RX                   8         // Recieving data
#define SERIAL_TMC_TX                   6         // Transmit data
#define SERIAL_TMC_ADDRESS_MAP(x)       0         // Axis4(3) is 0

#define STATUS_LED_PIN                 15
#define STATUS_LED_ON_STATE          HIGH

// Specify the ESP32-S2 I2C
// actual pins 17 and 18 respectively 
#define I2C_SDA_PIN                    21
#define I2C_SCL_PIN                    22

// For focuser1 stepper driver
#define AXIS4_ENABLE_PIN                4
#define AXIS4_M0_PIN                  OFF         // hardwired for TMC UART address 0
#define AXIS4_M1_PIN                  OFF         // hardwired for TMC UART address 0
#define AXIS4_M2_PIN                  OFF         // N/C
#define AXIS4_M3_PIN                  OFF         // N/C
#define AXIS4_STEP_PIN                 10
#define AXIS4_DIR_PIN                  13

// PINMAP ------------------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#PINMAP
#define PINMAP                        OFF //    OFF, Choose from: MiniPCB, MiniPCB2, MaxPCB2, MaxESP3, CNC3, STM32Blue,      <-Req'd
                                          //         MaxSTM3, FYSETC_S6_2, etc.  Other boards and more info. in ~/src/Constants.h

// SERIAL PORT COMMAND CHANNELS --------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#SERIAL_PORTS
#define SERIAL_A_BAUD_DEFAULT      230400 //   9600, n. Where n=9600,19200,57600,115200,230400,460800 (common baud rates.)    Infreq
#define SERIAL_B_BAUD_DEFAULT         OFF //   9600, n. Baud rate as above. See (src/pinmaps/) for Serial port assignments.   Infreq
#define SERIAL_B_ESP_FLASHING         OFF //    OFF, ON Upload ESP8266 WiFi firmware through SERIAL_B with :ESPFLASH# cmd.    Option
#define SERIAL_C_BAUD_DEFAULT         OFF //    OFF, n. Baud rate as above. See (src/pinmaps/) for Serial port assignments.   Infreq
#define SERIAL_D_BAUD_DEFAULT         OFF //    OFF, n. Baud rate as above. See (src/pinmaps/) for Serial port assignments.   Infreq
#define SERIAL_E_BAUD_DEFAULT         OFF //    OFF, n. Baud rate as above. See (src/pinmaps/) for Serial port assignments.   Infreq
#define SERIAL_RADIO    WIFI_ACCESS_POINT //    OFF, Use BLUETOOTH or WIFI_ACCESS_POINT or WIFI_STATION (ESP32 only.)         Option

// STATUS --------------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#STATUS_LED
#define STATUS_LED                     ON //    OFF, Steady illumination if no error, blinks w/error code otherwise.          Option

// RETICLE CONTROL ------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#RETICLE_CONTROL
#define RETICLE_LED_DEFAULT           OFF //    OFF, n. Where n=0..255 (0..100%) activates feature sets default brightness.   Option
#define RETICLE_LED_MEMORY            OFF //    OFF, ON Remember reticle brightness across power cycles.                      Option
#define RETICLE_LED_INVERT            OFF //    OFF, ON Inverts control for cases where 0V is max brightness.                 Option

// WEATHER SENSOR --------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#WEATHER_SENSOR
#define WEATHER                       OFF //    OFF, BME280 (I2C 0x77,) BME280_0x76, BME280_SPI (see pinmap for CS.)          Option
                                          //         BMP280 (I2C 0x77,) BMP280_0x76, BMP280_SPI (see pinmap for CS.)
                                          //         BME280 or BMP280 for temperature, pressure.  BME280 for humidity also.

// SIGNALING ------------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#SIGNALING
#define STEP_WAVE_FORM              PULSE // SQUARE, PULSE Step signal wave form faster rates. SQUARE best signal integrity.  Adjust
                                          //         Applies to all axes.

// NON-VOLATILE MEMORY ---------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Controller#NV
#define NV_DRIVER              NV_DEFAULT // NV_DEF, Use platforms default non-volatile device to remember runtime settings.  Option

// =================================================================================================================================
// FOCUSERS ========================================================================================================================

// Driver models (Step/Dir and Servo) many have specific requirements so be sure to follow the link below to help learn about these.
// Typically: A4988, DRV8825, LV8729, S109, TMC2130S, etc.

// AXIS4 FOCUSER 1 -------------------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Focuser
#define AXIS4_DRIVER_MODEL        TMC2209 //    OFF, Enter motor driver model (above) to activate the focuser.                Option
#define AXIS4_SLEW_RATE_BASE_DESIRED  500 //    500, n, Where n=200..5000 (um/s.) Adjustable at run-time from                <-Req'd
                                          //         1/2 to 2x this rate, and as performance considerations require.
#define AXIS4_SLEW_RATE_MINIMUM        20 //     20, n. Where n=5..200 (um/s.) Minimum microns/second.                        Adjust

// If runtime axis settings are enabled changes in the section below will be ignored (disable in SWS or by wiping NV/EEPROM):
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/
#define AXIS4_STEPS_PER_MICRON        0.5 //    0.5, n. Steps per micrometer. Figure this out by testing or other means.      Adjust
#define AXIS4_REVERSE                 OFF //    OFF, ON Reverses movement direction, or reverse wiring instead to correct.    Option
#define AXIS4_LIMIT_MIN                 0 //      0, n. Where n=0..500 (millimeters.) Minimum allowed position.               Adjust
#define AXIS4_LIMIT_MAX                50 //     50, n. Where n=0..500 (millimeters.) Maximum allowed position.               Adjust

#define AXIS4_DRIVER_MICROSTEPS         8 //    OFF, n. Microstep mode when tracking.                                         Option
#define AXIS4_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n. Microstep mode used during slews. OFF uses _DRIVER_MICROSTEPS.        Option

// for TMC2130, TMC5160, TMC2209, TMC2226 STEP/DIR driver models:
#define AXIS4_DRIVER_IHOLD            OFF //    OFF, n, (mA.) Current during standstill. OFF uses IRUN/2.0                    Option
#define AXIS4_DRIVER_IRUN             500 //    OFF, n, (mA.) Current during tracking, appropriate for stepper/driver/etc.    Option
#define AXIS4_DRIVER_IGOTO            OFF //    OFF, n, (mA.) Current during slews. OFF uses IRUN.                            Option
// /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /

#define AXIS4_DRIVER_STATUS            ON //    OFF, ON, HIGH, or LOW.  For driver status info/fault detection.               Option

#define AXIS4_DRIVER_DECAY      STEALTHCHOP //  OFF, Tracking decay mode default override. TMC default is STEALTHCHOP.        Infreq
#define AXIS4_DRIVER_DECAY_GOTO STEALTHCHOP //  OFF, Decay mode goto default override. TMC default is SPREADCYCLE.            Infreq

#define AXIS4_POWER_DOWN              OFF //    OFF, ON Powers off 30 seconds after movement stops.                           Option

#define AXIS4_SENSE_HOME              OFF //    OFF, HIGH or LOW enables & state clockwise home position, as seen from above. Option
#define AXIS4_SENSE_LIMIT_MIN         OFF //    OFF, HIGH or LOW state on limit sense switch stops movement.                  Option
#define AXIS4_SENSE_LIMIT_MAX         OFF //    OFF, HIGH or LOW state on limit sense switch stops movement.                  Option

// AXIS5 FOCUSER 2 -----------------------------------------------------------------------------------------------------------------
// Up to 6 focusers can be present (AXIS4 to AXIS9) simply copy the above text for focuser 1 and rename to AXIS5_ for focuser 2, etc

// FOCUSER TEMPERATURE ---------------------------------------------- see https://onstep.groups.io/g/main/wiki/Configuration_Focuser
#define FOCUSER_TEMPERATURE    THERMISTOR //    OFF, THERMISTOR or n. Where n is the ds18b20 s/n for focuser temp.            Adjust

// ---------------------------------------------------------------------------------------------------------------------------------
#define FileVersionConfig 6
#include "Extended.config.h"
