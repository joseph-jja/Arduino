// OnstepX Plugin 4 SHC Time / Date
#pragma once

#if defined(TIME_LOCATION_SOURCE) && TIME_LOCATION_SOURCE != OFF

//#include "../../lib/commands/CommandErrors.h"
#include "../../lib/Constants.h"
#include "../../lib/tls/Tls.h"
#include "../../libApp/cmdLx200/CmdLx200.h"
#include "../../lib/calendars/Calendars.h"

#define RESPONSE_BUFFER_SIZE 50
#define INVALID_OFFSET 100;
#define INVALID_LOCATION 10000;
#define INVALID_DATETIME -1;

class OnstepxSHCTimePlugin {

public:
  // the initialization method must be present and named: void init();
  void init();

  void loop();

  //bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError);

private:

     char response_buffer[RESPONSE_BUFFER_SIZE];

      TimeLocationSource *tls;
};

extern OnstepxSHCTimePlugin onstepxSHCTimePlugin;
#endif

