#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepXMountPlugin.h"

//#include "../../lib/tasks/OnTask.h"

void OnStepXMountPlugin::init() {

    for ( int i = 0; i < MAX_SITES; i++) {
        memset(siteInfo[i].sitename, '\0', SITE_NAME_LENGTH);
        siteInfo[i].latitude.sign = '+';
        siteInfo[i].latitude.hours = -1;
        siteInfo[i].latitude.minutes = -1;
        siteInfo[i].longitude.sign = '+';
        siteInfo[i].longitude.hours = -1;
        siteInfo[i].longitude.minutes = -1;
        siteInfo[i].utc_offset.sign = '+';
        siteInfo[i].utc_offset.hours = 0;
    }

    parse_location(DEFAULT_LATITUDE, &latitude, MAX_LATITUDE);
    parse_location(DEFAULT_LONGITUDE, &longitude, MAX_LONGITUDE);

    parse_offset(DEFAULT_UTC_OFFSET, &utc_offset);

    parse_date(DEFAULT_DATE, &dateInfo);
    parse_time(DEFAULT_TIME, &timeInfo);
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

   // this is the ACK command
   // not handled here
   /*if (command[0] == (char)6) {
      *numericReply = false;
      *supressFrame = false;
      sprintf(reply, MOUNT_MODE);
      return true;
   }*/

    if(command[0] == 'G') {
      memset(response_buffer, '\0', RESPONSE_BUFFER_SIZE);
      bool override = false;
       if (command[1] == 't') {
         override = true;
         location_toString(latitude, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'g') {
         override = true;
         location_toString(longitude, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'G') {
         override = true;
         offset_toString(utc_offset, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'X' && parameter[0] == '9' && parameter[1] == '8') {
          override = true;
          sprintf(response_buffer, "N");
       } else if (command[1] == 'C') {
          // GC get date
           override = true;
          date_toString(dateInfo, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'L') {
          // get local time in 24h format
          override = true;
          time_24h_toString(timeInfo, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'a') {
          // get local time in 12h format
          override = true;
          time_12h_toString(timeInfo, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'S') {
          // get sidereal time
          override = true;
          time_24h_toString(siderealTimeInfo, response_buffer, RESPONSE_BUFFER_SIZE);
       } else if (command[1] == 'M') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", siteInfo[0].sitename);
       } else if (command[1] == 'N') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", siteInfo[1].sitename);
       } else if (command[1] == 'O') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", siteInfo[2].sitename);
       } else if (command[1] == 'P') {
          // GM get site name
          override = true;
          sprintf(response_buffer, "%s", siteInfo[3].sitename);
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
         if (!parse_location(parameter, &latitude, MAX_LATITUDE)) {
              *commandError = CommandError::CE_PARAM_FORM;
         }
       } else if (command[1] == 'g') {
         override = true;
         if (!parse_location(parameter, &longitude, MAX_LONGITUDE)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'G') {
         override = true;
         if (!parse_offset(parameter, &utc_offset)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'C') {
          // SC set date
          override = true;
          if (!parse_date(parameter, &dateInfo)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'L') {
          // set local time
          override = true;
          if (!parse_time(parameter, &timeInfo)) {
              *commandError = CommandError::CE_PARAM_FORM;
          }
       } else if (command[1] == 'S') {
          // set sidereal time
          override = true;
          if (!parse_time(parameter, &siderealTimeInfo)) {
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
