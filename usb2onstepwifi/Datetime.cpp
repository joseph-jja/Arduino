#include <stdlib.h>
#include <string.h>

#include "Datetime.h"

#define DATE_TIME_SIZE 10

void Datetime::init(unsigned long ms) {

  start_time = ms;
  delta_time = start_time;
}

bool validate_string(char buffer[], char v) {
  int len = strlen(buffer);
  if (buffer[len - 1] != '#' || len < 9) {
    return false;
  }

  if (buffer[2] != v || buffer[5] != v) {
    return false;
  }
}

long copy_buffer(char str_in[], long m1, long m2, long m3) {

  char buffer[DATE_TIME_SIZE];

  long value = 0;
  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[0];
  buffer[1] = str_in[1];
  value += (atol(buffer) * m1);

  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[3];
  buffer[0] = str_in[4];
  value += (atol(buffer) * m2);

  memset(buffer, '\0', DATE_TIME_SIZE);
  buffer[0] = str_in[6];
  buffer[1] = str_in[7];
  value += (atol(buffer) * m3);

  return value;
}

// mm/dd/yy
bool Datetime::set_date(char date_str[]) {

  if (!validate_string(date_str, '/')) {
    return false;
  }

  // TODO fix me to use correct
  date_part = copy_buffer(date_str, 1, 1, 1);

  return true;
}

bool Datetime::set_time(char time_str[]) {

  if (!validate_string(time_str, ':')) {
    return false;
  }

  // convert time string to actual number
  time_part = copy_buffer(time_str, 3600, 60, 1);

  return true;
}

void Datetime::get_date(char date[], unsigned long ms) {

  long current_time = ((date_part * 1000000) + time_part) * 1000;  //milliseconds

  /*  delta_time = ms - start_time;
  start_time = ms;
  current_time += delta_time;
  return current_time;*/
}

void Datetime::get_time(char time[], unsigned long ms) {

  long current_time = ((date_part * 1000000) + time_part) * 1000;  //milliseconds
}