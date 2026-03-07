#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepXMountPlugin.h"

//#include "../../lib/tasks/OnTask.h"

// intitialize all values to invalid values
void OnstepxSHCTimePlugin::init() {

  latitude.sign = '';
  latitude.hours = INVALID_LOCATION;
  latitude.minutes = INVALID_LOCATION;

  longitude.sign = '';
  longitude.hours = INVALID_LOCATION;
  longitude.minutes = INVALID_LOCATION;

  // not valid value
  offset = INVALID_OFFSET;

  // invalid date
  dateInfo.year = INVALID_DATETIME;
  dateInfo.month = INVALID_DATETIME;
  dateInfo.day = INVALID_DATETIME;
  
  timeInfo.hour = INVALID_DATETIME;
  timeInfo.minutes = INVALID_DATETIME;
  timeInfo.seconds = INVALID_DATETIME;
  //  char meridian[3];
};

void OnstepxSHCTimePlugin::loop(){
  #ifdef TIME_LOCATION_SOURCE

  #endif
};

OnStepXMountPlugin onstepxMountPlugin;
