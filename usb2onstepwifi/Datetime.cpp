#include <string.h>

#include "Datetime.h"

void Datetime::init(unsigned long ms) {

  memset(date_str, '\0', DATE_TIME_SIZE);
  memset(time_str, '\0', DATE_TIME_SIZE);

  // date and time stuff
  char date_str[DATE_TIME_SIZE];
  char time_str[DATE_TIME_SIZE];

  start_time = ms;
  delta_time = start_time;
}

bool Datetime::set_date(char date[]) {

  int len = strlen(date);

  return true;
}

bool Datetime::set_time(char time[]) {

  int len = strlen(time);

  return true;
}

void Datetime::get_date(char date[]) {
}

void Datetime::get_time(char time[]) {
}