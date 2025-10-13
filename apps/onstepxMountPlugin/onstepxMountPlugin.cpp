#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "onstepxMountPlugin.h"

#include "../../Common.h"
#include "../../lib/tasks/OnTask.h"

#include "../../lib/ethernet/webServer/WebServer.h"
#include "../../lib/wifi/webServer/WebServer.h"

void OnStepXMountPlugin::init() {
    
  memset(latitude, '\0', DEFAULT_LOCATION_SIZE);
  memset(longitude, '\0', DEFAULT_LOCATION_SIZE);
  memset(  utcoffset = DEFAULT_UTC_OFFSET;, '\0', DEFAULT_LOCATION_SIZE);

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE, strlen(DEFAULT_LONGITUDE));
  memcpy(utcoffset, DEFAULT_UTC_OFFSET, strlen(DEFAULT_LONGITUDE));

  // TODO incorporate date stuff and other special commands
  // can we handle the ACK here?
}

void OnStepXMountPlugin::loop() {

}

void OnStepXMountPlugin::bool command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError) {

    if(command[0] == 'G') {
       *numericReply = false;
       if (command[1] == 't') {
         sprintf(reply, latitude);
       } else if (command[1] == 'g') {
         sprintf(reply, longitude);
         return true;
       }
    } else if(command[0] == 'S') {
       *numericReply = true;
       if (command[1] == 't') {
         //parameter
         return true;
       } else if (command[1] == 'g') {
         //parameter
         return true;
       }
    }
      
    return false;
}
