#include <RTClib.h>

#define SERIAL_BAUD 9600

// real time clock
RTC_DS3231 rtc;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(SERIAL_BAUD);

  if (rtc.begin()) {

  }
  rtc.enable32K();
  rtc.writeSqwPinMode(DS3231_SquareWave1Hz);

}
void loop() {

  /* probably do not need this
  if (!rtc.isEnabled32K()) {
    rtc.enable32K();
  }

  if (!rtc.readSqwPinMode() != DS3231_SquareWave1Hz) {
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
  }
  */
}
