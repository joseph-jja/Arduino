#include <stdlib.h>
#include <string.h>

#include "Datetime.h"

#define DATE_TIME_SIZE 10

void Datetime::init(unsigned long ms) {

  start_time = ms;
  delta_time = start_time;
}

bool validate_string(char buffer[]) {
  int len = strlen(buffer);
  if (buffer[len - 1] != '#' || len < 9) {
    return false;
  }

  if (buffer[2] != '/' || buffer[5] != '/') {
    return false;
  }
}

long copy_buffer(char str_in[]) {

  char buffer[DATE_TIME_SIZE];

  memset(buffer, '\0', DATE_TIME_SIZE);

  buffer[0] = str_in[0];
  buffer[1] = str_in[1];
  buffer[2] = str_in[3];
  buffer[3] = str_in[4];
  buffer[4] = str_in[6];
  buffer[5] = str_in[7];
  buffer[6] = str_in[8];
  return atol(buffer);
}

bool Datetime::set_date(char date_str[]) {

  if (!validate_string(date_str)) {
    return false;
  }

  date_part = copy_buffer(date_str);

  return true;
}

bool Datetime::set_time(char time_str[]) {

  if (!validate_string(time_str)) {
    return false;
  }

  time_part = copy_buffer(time_str);

  return true;
}

void Datetime::get_date(char date[], unsigned long ms) {

  //long current_time
  //date_part * 1000000
  //time_part
}

void Datetime::get_time(char time[], unsigned long ms) {

  //start_time

  //time_part
}