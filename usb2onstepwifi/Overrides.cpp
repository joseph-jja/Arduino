#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "Overrides.h"
#include "string_functions.h"

void Overrides::init() {

  memset(latitude, '\0', DEFAULT_LOCATION_SIZE);
  memset(longitude, '\0', DEFAULT_LOCATION_SIZE);

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE, strlen(DEFAULT_LONGITUDE));

  memset(date_str, '\0', DEFAULT_DATE_TIME_SIZE);
  memset(local_time_str, '\0', DEFAULT_DATE_TIME_SIZE);
  memset(time_str, '\0', DEFAULT_DATE_TIME_SIZE);

  memcpy(date_str, "09/12/25#", strlen("09/12/25#"));
  memcpy(local_time_str, "12:12:15#", strlen("12:12:15#"));
  memcpy(time_str, "12:12:15#", strlen("12:12:15#"));
};

/*
Set date    :SCMM/DD/YY#	Reply: 0 or 1
Get date	:GC#	Reply: MM/DD/YY#
Set time (Local)	:SLHH:MM:SS#	Reply: 0 or 1
Get time (Local, 12hr format)	:Ga#	Reply: HH:MM:SS#
Get time (Local, 24hr format)	:GL#	Reply: HH:MM:SS#
Set time (Sidereal)	:SSHH:MM:SS#	Reply: 0 or 1
Get time (Sidereal)	:GS#	Reply: HH:MM:SS#

Site/Location commands	 
Set UTC Offset(for current site)	:SGsHH#	Reply: 0 or 1
Get UTC Offset(for current site)	:GG#	Reply: sHH#
The UTC Offset value is the number of hours to add to your Local Time (Standard Time) to get Universal Time.	 
Set Latitude (for current site)	:StsDD*MM#	Reply: 0 or 1
Get Latitude (for current site)	:Gt#	Reply: sDD*MM#
Set Longitude (for current site)	:SgDDD*MM#	Reply: 0 or 1
Get Longitude (for current site)	:Gg#	Reply: DDD*MM#
*/
bool Overrides::check_override(char *bufferIn, char buffer[], int buffer_size) {

  bool override = false;

  if (bufferIn == NULL) {
    return override;
  }

  memset(buffer, '\0', buffer_size);
  int bufferInLen = strlen(bufferIn);

  if (bufferInLen == 0) {
    return override;
  }

  // commands to skip
  // date and time commands
  // latitude and lingitude commands and offset
  if (compare(bufferIn, ":Gt#")) {
    override = true;
    memcpy(buffer, latitude, strlen(latitude));
  } else if (compare(bufferIn, ":St")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, latitude);
    memcpy(buffer, "0", strlen("0"));
  } else if (compare(bufferIn, ":Gg#")) {
    override = true;
    memcpy(buffer, longitude, strlen(longitude));
  } else if (compare(bufferIn, ":Sg")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, longitude);
    memcpy(buffer, "0", strlen("0"));
  } else if (compare(bufferIn, ":GG#")) {
    override = true;
    char offsetBuff[5];
    memset(buffer, '\0', 5);
    sprintf(buffer, "%d", utcoffset);
  } else if (compare(bufferIn, ":SG")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, buffer);
    utcoffset = atoi(buffer);
    memcpy(buffer, "0", strlen("0"));
    // date functions
  } else if (compare(bufferIn, ":GC#")) {
    override = true;
    memcpy(buffer, date_str, strlen(date_str));
  } else if (compare(bufferIn, ":SC")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, date_str);
    memcpy(buffer, "0", strlen("0"));
    // local time functions
  } else if (compare(bufferIn, ":Ga#")) {
    override = true;
    memcpy(buffer, local_time_str, strlen(local_time_str));
  } else if (compare(bufferIn, ":GL#")) {
    override = true;
    memcpy(buffer, local_time_str, strlen(local_time_str));
  } else if (compare(bufferIn, ":SL")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, local_time_str);
    memcpy(buffer, "0", strlen("0"));
    // local time functions
  } else if (compare(bufferIn, ":GS#")) {
    override = true;
    memcpy(buffer, time_str, strlen(local_time_str));
  } else if (compare(bufferIn, ":SS")) {
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, time_str);
    memcpy(buffer, "0", strlen("0"));
  }

  return override;
}

Overrides overrides;

