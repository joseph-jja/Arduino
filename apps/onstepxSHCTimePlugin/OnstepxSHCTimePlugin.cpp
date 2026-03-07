#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepXMountPlugin.h"

//#include "../../lib/tasks/OnTask.h"

void OnstepxSHCTimePlugin::init() {

  latitude.sign = '';
  latitude.hours = INVALID_LOCATION;
  latitude.minutes = INVALID_LOCATION;

  longitude.sign = '';
  longitude.hours = INVALID_LOCATION;
  longitude.minutes = INVALID_LOCATION;

  // not valid value
  offset = INVALID_OFFSET;

  //dateInfo
  //timeInfo

};

void OnstepxSHCTimePlugin::loop(){

};

OnStepXMountPlugin onstepxMountPlugin;
