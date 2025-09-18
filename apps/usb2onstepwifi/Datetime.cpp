#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Datetime.h"

#define ONE_DAY 84000
#define DEFAULT_DATE 20250918

static char compile_time[] = __TIME__;

static int DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

bool validate_string(char buffer[], char v) {
  int len = strlen(buffer);
  if (buffer[len - 1] != '#' || len < 9) {
    return false;
  }

  if (buffer[2] != v || buffer[5] != v) {
    return false;
  }
  return true;
}

long convert_buffer(char str_in[], long m1, long m2, long m3) {

  char buffer[DATE_TIME_SIZE];

  long value = 0;
  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[0];
  buffer[1] = str_in[1];
  value += (atol(buffer) * m1);

  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[3];
  buffer[1] = str_in[4];
  value += (atol(buffer) * m2);

  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[6];
  buffer[1] = str_in[7];
  value += (atol(buffer) * m3);

  return value;
}

void Datetime::init(unsigned long ms) {

  start_time = ms;
  delta_time = start_time;
  days_changed = 0;  // add in to header file

  date_part = DEFAULT_DATE;
  local_time_part = convert_buffer(compile_time, 3600, 60, 1);
  sidereal_time_part = convert_buffer(compile_time, 3600, 60, 1);
}

// mm/dd/yy
bool Datetime::set_date(char date_str[]) {

  if (!validate_string(date_str, '/')) {
    return false;
  }

  // more normal format of date
  // YYYYMMDD
  date_part = convert_buffer(date_str, 100, 1, 10000) + 20000000;

  return true;
}

// time is always set as 24 hour time
bool Datetime::set_local_time(char time_str[]) {

  if (!validate_string(time_str, ':')) {
    return false;
  }

  // convert time string to actual number
  local_time_part = convert_buffer(time_str, 3600, 60, 1);

  return true;
}

// time is always set as 24 hour time
bool Datetime::set_sidereal_time(char time_str[]) {

  if (!validate_string(time_str, ':')) {
    return false;
  }

  // convert time string to actual number
  sidereal_time_part = convert_buffer(time_str, 3600, 60, 1);

  return true;
}

void Datetime::get_date(char date[], unsigned long ms) {

  // 2 digit year
  long year = floor(date_part / 10000) - 2000;
  long month = (date_part - ((2000 + year) * 10000)) / 100;
  long day = (date_part - ((2000 + year) * 10000)) - (month * 100);

  sprintf(date, "%02ld/%02ld/%02ld#", month, day, year);
}

long Datetime::update_time(unsigned long ms) {

  delta_time = ms - start_time;
  start_time = ms;

  long delta_time_now = ceil(delta_time / 1000);  //seconds

  return delta_time_now;
}

long Datetime::update_time_value(char time[], unsigned long ms, long time_in, bool is_twenty_four_hour) {

  long time_part = time_in;

  // update time
  time_part += update_time(ms);

  int i = -1;  // 0 day
  long value = time_part;
  while (value > 0) {
    value -= ONE_DAY;
    i++;
  }
  days_changed = i;

  // value would be negative now
  // so now we add back one day
  // to make positive and this is our new time
  // in seconds
  time_part = value + ONE_DAY;

  // convert time_part to hh:mm:ss
  long mod_time = time_part % 3600;
  long hours = time_part / 3600;
  long minutes = mod_time / 60;
  long seconds = mod_time % 60;
  if (!is_twenty_four_hour && hours > 12) {
    hours -= 12;
    sprintf(time, "%02ld:%02ld:%02ld#", hours, minutes, seconds);
  } else {
    sprintf(time, "%02ld:%02ld:%02ld#", hours, minutes, seconds);
  }

  return time_part;
}

void Datetime::get_local_time(char time[], unsigned long ms, bool is_twenty_four_hour) {

  local_time_part = update_time_value(time, ms, local_time_part, is_twenty_four_hour);
}

void Datetime::get_sidereal_time(char time[], unsigned long ms) {

  // update time
  sidereal_time_part = update_time_value(time, ms, sidereal_time_part, true);
}

Datetime datetime;
