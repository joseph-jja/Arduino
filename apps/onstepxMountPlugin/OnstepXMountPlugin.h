// USB2I2C plugin
#pragma once

#include "lib/location_functions.h"
#include "lib/datetime_functions.h"
#include "lib/site_functions.h"

#include "../../lib/commands/CommandErrors.h"

#define DEFAULT_LOCATION_SIZE 20

class OnStepXMountPlugin {

public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

     Site site;
     Location location;
     Datetime datetime;
    
};

extern OnStepXMountPlugin onstepxMountPlugin;
