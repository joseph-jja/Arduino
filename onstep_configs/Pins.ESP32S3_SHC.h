// -------------------------------------------------------------------------------------------------
// pinmap for ESP32
#pragma once

#if defined(ESP32)

  // no joystick 
  #define KEYPAD_JOYSTICK_ANALOG        OFF 
  
  #ifndef B_PIN0
    #define B_PIN0             15  // Shift
  #endif
  #ifndef B_PIN1
    #define B_PIN1             16  // N
  #endif
  #ifndef B_PIN2
    #define B_PIN2             17  // S
  #endif
  #ifndef B_PIN3
    #define B_PIN3             5  // E
  #endif
  #ifndef B_PIN4
    #define B_PIN4             9  // W
  #endif
  #ifndef B_PIN5
    #define B_PIN5             4  // F
  #endif
  #ifndef B_PIN6
    #define B_PIN6             11  // f
  #endif
  
  #define ST4_W_PIN            6  // ST4 (OnStep) RA- West,  send data to OnStep
  #define ST4_S_PIN            7  // ST4 (OnStep) DE- South, clock input to ISR
  #define ST4_N_PIN            18  // ST4 (OnStep) DE+ North, recv data from OnStep
  #define ST4_E_PIN            8  // ST4 (OnStep) RA+ East,  always 12.5 Hz square wave on this pin

  #if ST4_AUX_INTERFACE == ON
    #define ST4_AUX_W_PIN      10  // ST4 (Aux) RA- West
    #define ST4_AUX_S_PIN      12  // ST4 (Aux) DE- South
    #define ST4_AUX_N_PIN      13  // ST4 (Aux) DE+ North
    #define ST4_AUX_E_PIN      14   // ST4 (Aux) RA+ East
  #endif

  #ifndef UTILITY_LIGHT_PIN
    #define UTILITY_LIGHT_PIN  21  // GPIO13 used as PWM output to drive utility lamp LED
  #endif

  // not implemented 
  //#ifndef DISPLAY_RESET_PIN
    //#define DISPLAY_RESET_PIN    16  // GPIO16 used to reset some displays, i.e. the SSD1309
  //#endif

#endif
