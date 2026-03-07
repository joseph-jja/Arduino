#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepxSHCTimePlugin.h"

//#include "../../lib/tasks/OnTask.h"

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
  
  #if TIME_LOCATION_SOURCE != OFF
      //TimeLocationSource *tls;
      //tls.init();
  #endif
};

//tls.set(int year, int month, int day, int hour, int minute, int second) 

void OnstepxSHCTimePlugin::loop() {
  #ifdef TIME_LOCATION_SOURCE
    JulianDate jdate;

    //tls.get(&jdate);

    GregorianDate greggy;
  
  #endif
};

OnstepxSHCTimePlugin onstepxSHCTimePlugin;
