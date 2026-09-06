#pragma once

// change these to the pins of your setup
#define GPS_TX_PIN   33
#define GPS_RX_PIN   34

// comment for software serial 
// esp32 uses hardware serial
#define USE_HARDWARE_SERIAL true
#define GPS_SERIAL_NUMBER  2

//#define ENABLE_DEBUG_LOGS true

typedef struct {
    // data was updated
    bool updated;

    char message[40];
    
    // gps location
    double latitude;
    double longitude;

    // time and date 
    char gps_date[12];
    char gps_time[10];
} GPS_LOCATION;

