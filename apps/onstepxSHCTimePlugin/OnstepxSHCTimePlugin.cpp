#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Config.h"
#include "OnstepxSHCTimePlugin.h"

#include "../../lib/tasks/OnTask.h"

#if defined(TIME_LOCATION_SOURCE) && TIME_LOCATION_SOURCE != OFF && TIME_LOCATION_SOURCE == DS3231

void onstepxSHCTimePluginWrapper() {
  onstepxSHCTimePlugin.loop();
}

// intitialize all values to invalid values
void OnstepxSHCTimePlugin::init() {

  tls = new TlsDs3231;
  tls->init();
  tasks.add(1000, 0, true, 7, onstepxSHCTimePluginWrapper);
};

void OnstepxSHCTimePlugin::loop() {
  JulianDate jdate;

  // get the date from the mount
  char out[20];
  onStepLx200.Get(":GC#", out);

  char* pEnd;
  uint8_t month = strtol(&out[0], &pEnd, 10);
  uint8_t day = strtol(&out[3], &pEnd, 10);
  uint8_t year = strtol(&out[6], &pEnd, 10);

  // now get the date from the RTC
  if (!tls->isReady()) {
    return;
  }

  tls->get(jdate);

  GregorianDate greggy = calendars.julianToGregorian(jdate);

  uint8_t mount_date = (year * 10000) + (month * 100) + day;
  uint8_t rtc_date = (greggy.year * 10000) + (greggy.month * 100) + greggy.day;

  // compare dates and use ours if greater
  if (rtc_date > mount_date) {
    // we should use this year, month and day
    snprintf(out, sizeof(out), ":SC%02d/%02d/%02d#", greggy.month, greggy.day, greggy.year);
    onStepLx200.Set(out);
  }

  // get sidereal time
  onStepLx200.Get(":GS#", out);
  

  //   tls.set(int year, int month, int day, int hour, int minute, int second);

};

OnstepxSHCTimePlugin onstepxSHCTimePlugin;
#endif
