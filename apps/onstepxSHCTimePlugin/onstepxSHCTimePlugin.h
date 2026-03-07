// USB2I2C plugin
#pragma once

#include "lib/location_functions.h"
#include "lib/datetime_functions.h"
#include "lib/site_functions.h"

#include "../../lib/commands/CommandErrors.h"

#define RESPONSE_BUFFER_SIZE 50

class OnStepXMountPlugin {

public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

     char response_buffer[RESPONSE_BUFFER_SIZE];

     SiteInfo   siteInfo[4];
     Location   latitude;
     Location   longitude;
     Offset     utc_offset;
     DateInfo   dateInfo;
     TimeInfo   timeInfo;
     TimeInfo   siderealTimeInfo;
};

extern OnStepXMountPlugin onstepxMountPlugin;
