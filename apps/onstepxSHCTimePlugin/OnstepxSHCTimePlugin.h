// OnstepX Plugin 4 SHC Time / Date
#pragma once

#include "lib/location_functions.h"
#include "lib/datetime_functions.h"
#include "lib/site_functions.h"

#include "../../lib/commands/CommandErrors.h"
#include "../../lib/Constants.h"
#include "../../lib/tls/Tls.h"

#define RESPONSE_BUFFER_SIZE 50

class OnstepxSHCTimePlugin {

public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  //bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

     char response_buffer[RESPONSE_BUFFER_SIZE];

     Location   latitude;
     Location   longitude;
     Offset     utc_offset;
     DateInfo   dateInfo;
     TimeInfo   timeInfo;
};

extern OnstepxSHCTimePlugin onstepxSHCTimePlugin;
