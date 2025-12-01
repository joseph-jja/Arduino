#pragma once

// change these to the pins of your setup
#define GPS_TX_PIN   33
#define GPS_RX_PIN   34

typedef struct {
    // data was updated
    bool updated;
    
    // gps location
    float latitude;
    float longitude;

    // time and date 
    char gps_date[12];
    char gps_time[10];
} GPS_LOCATION;

