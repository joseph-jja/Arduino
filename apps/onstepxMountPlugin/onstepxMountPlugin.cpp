#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "onstepxMountPlugin.h"
#include "lib/Location.h"

#include "../../Common.h"
#include "../../lib/tasks/OnTask.h"

#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

void OnStepXMountPlugin::init() {
  
  siteLocation.init();
  
  
}

void OnStepXMountPlugin::loop() {

}

void OnStepXMountPlugin::bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError) {

    if(command[0] == 'G') {
       if (command[1] == 't') {
         *numericReply = false;
         siteLocation.location_toString(&siteLocation.latitude, siteLocation.location_buffer, DEFAULT_LOCATION_SIZE);
         sprintf(reply, siteLocation.location_buffer);
       } else if (command[1] == 'g') {
         *numericReply = false;
         siteLocation.location_toString(&siteLocation.longitude, siteLocation.location_buffer, DEFAULT_LOCATION_SIZE);
         sprintf(reply, siteLocation.location_buffer);
         return true;
       }
    } else if(command[0] == 'S') {
       if (command[1] == 't') {
        *numericReply = true;
         return true;
       } else if (command[1] == 'g') {
        *numericReply = true;
        return true;
       }
    }
      
    return false;
}
