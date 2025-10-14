#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepXMountPlugin.h"
#include "lib/Location.h"
#include "lib/Datetime.h"
#include "lib/Site.h"

//#include "../../lib/tasks/OnTask.h"

#define RESPONSE_BUFFER_SIZE 50

void OnStepXMountPlugin::init() {
  
  siteLocation.init();
  datetime.init();
  site.init();
};

void OnStepXMountPlugin::loop() {

};

/*
  This is where onstep plugins handle commands
  return true if the command was handled, false to pass it on to the next handler
  reply is the buffer to write the response to, when a reply that is not 0 or 1 is needed
  command is the command coming 
    => minus the leading : 
    => if the command is a set command then the command does not contain the data part of what was sent 
  parameter is the parameter part of the command or the data, it is valid for set commands
  suppressFrame is a bool and if false a # is added to the end of the reply, if true no # is added
  numericReply is a bool and if true 
    => the commandError parameter is interpreted and generates a '1' or '0' for success or failure
  commandError is a pointer to a CommandError enum
*/
bool OnStepXMountPlugin::command(char reply[], char command[], char parameter[], bool *supressFrame, bool *numericReply, CommandError *commandError) {

    if(command[0] == 'G') {
      char response_buffer[RESPONSE_BUFFER_SIZE];
      memset(response_buffer, '\0', RESPONSE_BUFFER_SIZE);
      bool override = false;
       if (command[1] == 't') {
         override = true;
         siteLocation.location_toString(siteLocation.latitude, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'g') {
         override = true;
         siteLocation.location_toString(siteLocation.longitude, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'G') {
         override = true;
         siteLocation.offset_toString(siteLocation.utc_offset, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'X' && parameter[0] == '9' && parameter[1] == '8') {
          override = true;
          sprintf(response_buffer, "N");
       } else if (command[1] == 'C') {
          // GC get date
           override = true;
          datetime.date_toString(datetime.date_part, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'L') {
          // get local time in 24h format
          override = true;
          datetime.time_24h_toString(datetime.local_time_part, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'a') {
          // get local time in 12h format
          override = true;
          datetime.time_12h_toString(datetime.local_time_part, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'S') {
          // get sidereal time
          override = true;
          datetime.time_24h_toString(datetime.sidereal_time_part, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'M') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", site.siteInfo[0].sitename);
       } else if (command[1] == 'N') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", site.siteInfo[1].sitename);
       } else if (command[1] == 'O') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", site.siteInfo[2].sitename);
       } else if (command[1] == 'P') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", site.siteInfo[3].sitename);
       } else if (command[1] == 'T') {
          // tracking rate
          override = true;
          sprintf(response_buffer, "60.164");
       }

       // if we have a valid override command, return the response
       if (override) {
          *numericReply = false;
          *supressFrame = false;
          sprintf(reply, response_buffer);
          return true;
       }

    } else if(command[0] == 'S') {
       bool override = false;
       *commandError = CommandError::CE_NONE;
       if (command[1] == 't') {
         override = true;
         if (!siteLocation.parse_location(parameter, &siteLocation.latitude, MAX_LATITUDE)) {
              *commandError = CommandError::CE_PARAM_FORM;
         }
       } else if (command[1] == 'g') {
         override = true;
         if (!siteLocation.parse_location(parameter, &siteLocation.longitude, MAX_LONGITUDE)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'G') {
         override = true;
         if (!siteLocation.parse_offset(parameter, &siteLocation.utc_offset)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'C') {
          // SC set date
          override = true;
          if (!datetime.parse_date(parameter, &datetime.date_part)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'L') {
          // set local time
          override = true;
          if (!datetime.parse_time(parameter, &datetime.local_time_part)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'S') {
          // set sidereal time
          override = true;
          if (!datetime.parse_time(parameter, &datetime.sidereal_time_part)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       }
       // TODO handle setting sites M, N, O, P based on current lat, long, utc offset

       // if we have a valid override command, return the response
       if (override) {
          *numericReply = true;
          *supressFrame = true;
          return true;
       }
    }
      
    return false;
};

OnStepXMountPlugin onstepxMountPlugin;
