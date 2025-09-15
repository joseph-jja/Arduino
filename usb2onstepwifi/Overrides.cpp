#include <string.h>

#include "Config.h"
#include "Overrides.h"

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

bool Overrides::check_override(char *bufferIn) {
  return true;
};