#include "gps_config.h"

// gps
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// The TinyGPS object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial softwareSerial(GPS_RX_PIN, GPS_TX_PIN);

GPS_LOCATION gpsdata;

void setup_gps() {

    Serial.print("GPS GT-U7 setup using Tiny GPS library version ");
    Serial.println(TinyGPSPlus::libraryVersion());
    softwareSerial.begin(GPSBaud);
}

void loop_gps() {

  long year = 0,
    month = 0,
    day = 0,
    hours = 0,
    minutes = 0,
    seconds = 0;

  while (softwareSerial.available() > 0) {
        //Serial.println("Trying to get GPS data.");
        if (gps.encode(softwareSerial.read())) {
            Serial.print("GPS data read using ");
            Serial.print(gps.satellites.value());
            Serial.println(" satellites.");

            if (gps.location.isValid() || gps.location.isUpdated()) {
                gpsdata.latitude = gps.location.lat();
                gpsdata.longitude = gps.location.lng();
                Serial.print("Got valid latitude and longitude ");
                Serial.print(gpsdata.latitude, 6);
                Serial.print(", ");
                Serial.print(gpsdata.longitude, 6);
                Serial.println("");
                delay(1000);
            }
            if (gps.time.isValid() || gps.time.isUpdated()) {
                hours = gps.time.hour();
                minutes = gps.time.minute();
                seconds = gps.time.seconds();
                memset(gpsdata.gps_time, '\0', sizeof(gpsdata.gps_date));
                sprintf(gpsdata.gps_time, "%02d:%02d:%02d", hours, minutes, seconds);
                Serial.print("Got raw time: ");
                Serial.print(hours);
                Serial.print(":");
                Serial.print(minutes);
                Serial.print(":");
                Serial.println(seconds);
                Serial.print("Got cookied time: ");
                Serial.println(gpsdata.gps_time);
            }
            if (gps.date.isValid() || gps.date.isUpdated()) {
                memset(gpsdata.gps_date, '\0', sizeof(gpsdata.gps_date));
                year = gps.date.year();
                month = gps.date.month();
                day = gps.date.day();
                sprintf(gpsdata.gps_date, "%4d/%02d/%02d", year, month, day);
                Serial.print("Got valid date ");
                Serial.println(gpsdata.gps_date);
            }
        }
    }
}

// get the DPS data 
GPS_LOCATION getGPSDate() {
    return gpsdata;
}

