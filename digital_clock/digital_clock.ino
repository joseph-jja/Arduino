#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <TimeLib.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <RTClib.h>

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

#define SERIAL_BAUD 9600

// real time clock
RTC_PCF8523 rtc;

void setup()
{
  
    // it does not seem to work unless you pass in the address?
    alpha4.begin(0x70);
    alpha4.setBrightness(5);

    for (int i = 0; i < 4; i++) {
        alpha4.writeDigitRaw(i, 0xFFFF);
        alpha4.writeDisplay();
    }
  
    delay(250);
    for (int i = 0; i < 4; i++) {
        alpha4.writeDigitRaw(i, 0x0);
        alpha4.writeDisplay();
    }

    Serial.begin(SERIAL_BAUD);
    Serial.println("Application setup!");

    // clock found so initialize to current data time from computer
    if (rtc.begin()) {
        if (!rtc.initialized()) {
            // following line sets the RTC to the date & time this sketch was compiled
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }
}

void get_time() {


    time_t tm = now();
    int hours = hour(tm);
    int minutes = minute(tm);

    char ampm[4];

    memset(ampm, '\0', sizeof(ampm) + 1);
    sprintf(ampm, "%2d%2d", hours, minutes);

    Serial.println(ampm);
        
    // no rtc so just pull time from thin air
    if (!rtc.begin()) {
        return;
    }
  
    if (!rtc.initialized()) {
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    } else {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    DateTime rtnow = rtc.now();
    Serial.print(rtnow.year(), DEC);
    Serial.print('/');
    Serial.print(rtnow.month(), DEC);
    Serial.print('/');
    Serial.print(rtnow.day(), DEC);
    Serial.print(' ');
    Serial.print(rtnow.hour(), DEC);
    Serial.print(':');
    Serial.print(rtnow.minute(), DEC);
    Serial.print(':');
    Serial.println(rtnow.second(), DEC);
    
    // set time
    setTime(rtnow.hour(), 
        rtnow.minute(),
        rtnow.second(),
        rtnow.day(),
        rtnow.month(),
        rtnow.year());
    
}

// method to write a long to screen
// 4 characters like 1000 if type is d
// 3 characters and type in space 0 otherwise
// 1 second delay after write
void writeString(long value, char type)
{
  int i;
  int len;
  char data[(sizeof(long) * 4) + sizeof(char) + 1];

  bool isTime = (type == 'd');

  memset(&data, '\0', sizeof(data));
  if (isTime) {
    sprintf(data, "%d", value);
  } else {
    sprintf(data, "%c%3d", type, value);
  }
  Serial.println(data);
  len = strlen(data);

  alpha4.clear();
  alpha4.writeDisplay();

  i = 0;

  // 100 i will be 0, 1, then 0 then 0 
  while (i < 4)
  {
    if ( i >= len )
    {
      break;
    }
    bool writeDot = (isTime & i == 1);
    alpha4.writeDigitAscii(i, data[i], writeDot);
    i++;
  }
  alpha4.writeDisplay();
  delay(1000);
}

long currentHour, 
   currentMinute;

void loop() {

    char buff[255];

    get_time();

    //writeString(1230, "x");
    //writeString(230, "d");
  
    delay(1000);

}
