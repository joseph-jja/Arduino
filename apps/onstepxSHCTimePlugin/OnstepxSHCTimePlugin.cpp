#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepxSHCTimePlugin.h"

#include "../../lib/tasks/OnTask.h"

void onstepxSHCTimePluginWrapper() { onstepxSHCTimePlugin.loop(); }

// intitialize all values to invalid values
void OnstepxSHCTimePlugin::init() {

  latitude.sign = ' ';
  latitude.hours = INVALID_LOCATION;
  latitude.minutes = INVALID_LOCATION;

  longitude.sign = ' ';
  longitude.hours = INVALID_LOCATION;
  longitude.minutes = INVALID_LOCATION;

  // not valid value
  utc_offset.sign = ' ';
  utc_offset.hours = INVALID_OFFSET;

  // invalid date
  dateInfo.year = INVALID_DATETIME;
  dateInfo.month = INVALID_DATETIME;
  dateInfo.day = INVALID_DATETIME;
  
  timeInfo.hour = INVALID_DATETIME;
  timeInfo.minutes = INVALID_DATETIME;
  timeInfo.seconds = INVALID_DATETIME;
  //  char meridian[3];
  
  #if defined(TIME_LOCATION_SOURCE) && TIME_LOCATION_SOURCE != OFF
      tls->init();
      tasks.add(1000, 0, true, 7, onstepxSHCTimePluginWrapper);
  #endif
};

void OnstepxSHCTimePlugin::loop() {
  #if defined(TIME_LOCATION_SOURCE) && TIME_LOCATION_SOURCE != OFF
    JulianDate jdate;

    // get the date from the mount
    char out[20];
    onStepLx200.Get(":GC#", out);
    
    char* pEnd;
    uint8_t month = strtol(&out[0], &pEnd, 10);
    uint8_t day = strtol(&out[3], &pEnd, 10);
    uint8_t year = strtol(&out[6], &pEnd, 10);
  
    // now get the date from the RTC
    //tls->get(jdate);
    
    GregorianDate greggy = calendars.julianToGregorian(jdate);

    // compare dates and use ours if greater
    if (greggy.year > year && greggy.month > month && greggy.day > day) {
        // we should use this year, month and day
        snprintf(out, sizeof(out), ":SC%02d/%02d/%02d#",  greggy.month, greggy.day, greggy.year);
        //onStepLx200.Set(out);
    }
    
    //tls.set(int year, int month, int day, int hour, int minute, int second) 
    
  
  #endif
};

OnstepxSHCTimePlugin onstepxSHCTimePlugin;
