#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "Overrides.h"
#include "string_functions.h"
#include "Datetime.h"

const char compile_time[] = __TIME__;

void Overrides::init(unsigned long ms) {

  memset(latitude, '\0', DEFAULT_LOCATION_SIZE);
  memset(longitude, '\0', DEFAULT_LOCATION_SIZE);

  memcpy(latitude, DEFAULT_LATITUDE, strlen(DEFAULT_LATITUDE));
  memcpy(longitude, DEFAULT_LONGITUDE, strlen(DEFAULT_LONGITUDE));

  utcoffset = DEFAULT_UTC_OFFSET;

  datetime.init(ms);
};

/*
Set date    :SCMM/DD/YY#  Reply: 0 or 1
Get date  :GC#  Reply: MM/DD/YY#
Set time (Local)  :SLHH:MM:SS#  Reply: 0 or 1
Get time (Local, 12hr format) :Ga#  Reply: HH:MM:SS#
Get time (Local, 24hr format) :GL#  Reply: HH:MM:SS#
Set time (Sidereal) :SSHH:MM:SS#  Reply: 0 or 1
Get time (Sidereal) :GS#  Reply: HH:MM:SS#

Site/Location commands   
Set UTC Offset(for current site)  :SGsHH# Reply: 0 or 1
Get UTC Offset(for current site)  :GG#  Reply: sHH#
The UTC Offset value is the number of hours to add to your Local Time (Standard Time) to get Universal Time.   
Set Latitude (for current site) :StsDD*MM#  Reply: 0 or 1
Get Latitude (for current site) :Gt#  Reply: sDD*MM#
Set Longitude (for current site)  :SgDDD*MM#  Reply: 0 or 1
Get Longitude (for current site)  :Gg#  Reply: DDD*MM#
*/
bool Overrides::check_override(char *bufferIn, char buffer[], int buffer_size, unsigned long ms) {

  bool override = false;

  if (bufferIn == NULL) {
    return override;
  }

  memset(buffer, '\0', buffer_size);
  int bufferInLen = strlen(bufferIn);

  if (bufferInLen == 0 || bufferInLen > buffer_size - 1) {
    return override;
  }

  char date_time_buffer[DATE_TIME_SIZE];
  memset(date_time_buffer, '\0', DATE_TIME_SIZE);

  // TODO lat and long should have . replaced with : if we get a .

  // commands to skip
  // date and time commands
  // latitude and lingitude commands and offset
  if (compare(bufferIn, ":Gt#")) {
    // get latitude
    override = true;
    memcpy(buffer, latitude, strlen(latitude));
  } else if (compare(bufferIn, ":St")) {
    // set latitude
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, latitude);
    replace_char(latitude, '.', ':');
    memcpy(buffer, "1", strlen("1"));
  } else if (compare(bufferIn, ":Gg#")) {
    // get longitude
    override = true;
    memcpy(buffer, longitude, strlen(longitude));
  } else if (compare(bufferIn, ":Sg")) {
    // set longitude
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, longitude);
    replace_char(longitude, '.', ':');
    memcpy(buffer, "1", strlen("1"));
  } else if (compare(bufferIn, ":GG#")) {
    // get utc offset
    override = true;
    sprintf(buffer, "%d", utcoffset);
  } else if (compare(bufferIn, ":SG")) {
    // set utc offset
    override = true;
    substring(bufferIn, 3, bufferInLen - 4, buffer);
    utcoffset = atoi(buffer);
    memset(buffer, '\0', buffer_size);
    memcpy(buffer, "1", strlen("1"));
  } else if (compare(bufferIn, ":GC#")) {
    // get date
    override = true;
    datetime.get_date(date_time_buffer, ms);
    sprintf(buffer, "%s", date_time_buffer);
  } else if (compare(bufferIn, ":SC")) {
    // set date
    override = true;
    substring(bufferIn, 3, bufferInLen - 3, date_time_buffer);
    bool success = datetime.set_date(date_time_buffer);
    sprintf(buffer, "%d", success);
  } else if (compare(bufferIn, ":Ga#")) {
    // get local time in 12 hour format
    override = true;
    datetime.get_local_time(date_time_buffer, ms, false);
    sprintf(buffer, "%s", date_time_buffer);
  } else if (compare(bufferIn, ":GL#")) {
    // get local time in 24 hour format
    override = true;
    datetime.get_local_time(date_time_buffer, ms, true);
    sprintf(buffer, "%s", date_time_buffer);
  } else if (compare(bufferIn, ":SL")) {
    // set local time, in 24 hour format
    override = true;
    substring(bufferIn, 3, bufferInLen - 3, date_time_buffer);
    bool success = datetime.set_local_time(date_time_buffer);
    sprintf(buffer, "%d", success);
  } else if (compare(bufferIn, ":GS#")) {
    // get sidereal time
    override = true;
    datetime.get_sidereal_time(date_time_buffer, ms);
    sprintf(buffer, "%s", date_time_buffer);
  } else if (compare(bufferIn, ":SS")) {
    // set sidereal time
    override = true;
    substring(bufferIn, 3, bufferInLen - 3, date_time_buffer);
    bool success = datetime.set_sidereal_time(date_time_buffer);
    sprintf(buffer, "%d", success);
  }

  return override;
}

Overrides overrides;
