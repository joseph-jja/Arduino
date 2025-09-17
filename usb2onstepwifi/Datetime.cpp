#include <stdlib.h>
#include <string.h>

#include "Datetime.h"

#define DATE_TIME_SIZE 10

void Datetime::init(unsigned long ms) {

  start_time = ms;
  delta_time = start_time;
}

bool Datetime::set_date(char date_str[]) {

  int len = strlen(date_str);
  if (date_str[len - 1] != '#' || len < 9) {
    return false;
  }

  if (date_str[2] != '/' || date_str[5] != '/') {
    return false;
  }

  char buffer[DATE_TIME_SIZE];
  memset(buffer, '\0', DATE_TIME_SIZE);

  buffer[0] = date_str[0];
  buffer[1] = date_str[1];
  buffer[2] = date_str[3];
  buffer[3] = date_str[4];
  buffer[4] = date_str[6];
  buffer[5] = date_str[7];
  buffer[6] = date_str[8];

  date_part = atoi(buffer);

  return true;
}

bool Datetime::set_time(char time_str[]) {

  int len = strlen(time_str);
  if (time_str[len - 1] != '#' || len < 9) {
    return false;
  }

  if (time_str[2] != '/' || time_str[5] != '/') {
    return false;
  }

  char buffer[DATE_TIME_SIZE];
  memset(buffer, '\0', DATE_TIME_SIZE);

  buffer[0] = time_str[0];
  buffer[1] = time_str[1];
  buffer[2] = time_str[3];
  buffer[3] = time_str[4];
  buffer[4] = time_str[6];
  buffer[5] = time_str[7];
  buffer[6] = time_str[8];

  time_part = atoi(buffer);

  return true;
}

void Datetime::get_date(char date[]) {
}

void Datetime::get_time(char time[]) {
}