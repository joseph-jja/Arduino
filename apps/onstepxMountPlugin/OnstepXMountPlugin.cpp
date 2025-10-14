#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepXMountPlugin.h"
#include "lib/Location.h"
#include "lib/Datetime.h"

//#include "../../lib/tasks/OnTask.h"

void OnStepXMountPlugin::init() {
  
  siteLocation.init();
  datetime.init();
}

void OnStepXMountPlugin::loop() {

}

bool OnStepXMountPlugin::command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError) {

    if(command[0] == 'G') {
       if (command[1] == 't') {
         //*numericReply = false;
         siteLocation.location_toString(siteLocation.latitude, siteLocation.location_buffer, DEFAULT_LOCATION_SIZE);
         sprintf(reply, siteLocation.location_buffer);
       } else if (command[1] == 'g') {
         //*numericReply = false;
         siteLocation.location_toString(siteLocation.longitude, siteLocation.location_buffer, DEFAULT_LOCATION_SIZE);
         sprintf(reply, siteLocation.location_buffer);
         return true;
       } else if (command[1] == 'G') {
         //*numericReply = false;
         siteLocation.offset_toString(siteLocation.utc_offset, siteLocation.location_buffer, DEFAULT_LOCATION_SIZE);
         sprintf(reply, siteLocation.location_buffer);
         return true;
       }
    } else if(command[0] == 'S') {
       if (command[1] == 't') {
         *numericReply = true;
         siteLocation.parse_location(parameter, &siteLocation.latitude);
         return true;
       } else if (command[1] == 'g') {
         *numericReply = true;
         siteLocation.parse_location(parameter, &siteLocation.longitude);
         return true;
       } else if (command[1] == 'G') {
         *numericReply = true;
         siteLocation.parse_offset(parameter, &siteLocation.utc_offset);
         return true;
       }
    }
      
    return false;
}

OnStepXMountPlugin onstepxMountPlugin;
