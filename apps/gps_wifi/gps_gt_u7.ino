// configuration
#include "Globals.h"

void get_gps_info() {

    long year = 0, month = 0, day = 0,
        currentHour = 0,
        currentMinute = 0;

    while (ss.available() > 0) {
        //Serial.println("Trying to get GPS data.");
        if (gps.encode(ss.read())) {
            blink_pin(100);
            Serial.print("GPS data read using ");
            Serial.print(gps.satellites.value());
            Serial.println(" satellites.");

            if (gps.location.isValid() || gps.location.isUpdated()) {
                latitude = gps.location.lat();
                longitude = gps.location.lng();
                Serial.print("Got valid latitude and longitude ");
                Serial.print(latitude, 6);
                Serial.print(", ");
                Serial.print(longitude, 6);
                Serial.println("");
                delay(1000);
            }
            if (gps.time.isValid() || gps.time.isUpdated()) {
                currentHour = gps.time.hour();
                currentMinute = gps.time.minute();
                memset(gps_time, '\0', sizeof(gps_date));
                sprintf(gps_time, "%02d:%02d", currentHour, currentMinute);
                Serial.print("Got raw time: ");
                Serial.print(currentHour);
                Serial.print(":");
                Serial.println(currentMinute);
                Serial.print("Got cookied time: ");
                Serial.println(gps_time);
            }
            if (gps.date.isValid() || gps.date.isUpdated()) {
                memset(gps_date, '\0', sizeof(gps_date));
                year = gps.date.year();
                month = gps.date.month();
                day = gps.date.day();
                sprintf(gps_date, "%4d/%02d/%02d", year, month, day);
                Serial.print("Got valid date ");
                Serial.println(gps_date);
            }
        }
    }
    delay(500);
}
