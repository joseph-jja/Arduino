#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepxSHCTimePlugin.h"

#include "../../lib/tasks/OnTask.h"

  #if defined(TIME_LOCATION_SOURCE) && TIME_LOCATION_SOURCE != OFF && TIME_LOCATION_SOURCE == DS3231

void onstepxSHCTimePluginWrapper() { onstepxSHCTimePlugin.loop(); }

// intitialize all values to invalid values
void OnstepxSHCTimePlugin::init() {

      tls = new TlsDs3231;
      tls->init();
      tasks.add(1000, 0, true, 7, onstepxSHCTimePluginWrapper);
};

void OnstepxSHCTimePlugin::loop() {
    JulianDate jdate;

    // get the date from the mount
    char out[20];
    onStepLx200.Get(":GC#", out);
    
    char* pEnd;
    uint8_t month = strtol(&out[0], &pEnd, 10);
    uint8_t day = strtol(&out[3], &pEnd, 10);
    uint8_t year = strtol(&out[6], &pEnd, 10);
  
    // now get the date from the RTC
    if (!tls->isReady()) {
      return;
    }
      
    tls->get(jdate);
  
    GregorianDate greggy = calendars.julianToGregorian(jdate);

    // compare dates and use ours if greater
    if (greggy.year > year && greggy.month > month && greggy.day > day) {
        // we should use this year, month and day
        snprintf(out, sizeof(out), ":SC%02d/%02d/%02d#",  greggy.month, greggy.day, greggy.year);
        //onStepLx200.Set(out);
    }
    
    //tls.set(int year, int month, int day, int hour, int minute, int second) 
    
  
};

OnstepxSHCTimePlugin onstepxSHCTimePlugin;
  #endif

