
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 10, TXPin = 11;
static const int GPSBaud = 9600;

// The TinyGPS object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

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

  Serial.begin(GPSBaud);
  Serial.println("Application setup!");
  Serial.println(TinyGPSPlus::libraryVersion());

  ss.begin(GPSBaud);

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

signed long tzOffset = -7;

bool gotGPSTime = false;
bool isLatLongValid = false;

void loop()
{

  char buff[255];
  
  long avail = ss.available();
  /*memset(&buff, '\0', sizeof(buff));
  sprintf(buff, "Start %d ", avail);
  Serial.println(buff);
  writeString(avail, 'A');*/

  isLatLongValid = false;
  gotGPSTime = false;

  while (avail > 0) {
    //Serial.println("Trying to get GPS data obtained");
    if (gps.encode(ss.read())) {
        Serial.println("GPS data obtained");
        //writeString(100, 'B');
        
        memset(&buff, '\0', sizeof(buff));
        sprintf(buff, "Latitude: %6d \t Longitude: %6d ", gps.location.lat(), gps.location.lng()); 
        Serial.println(buff);
        if (gps.location.isValid()) {
          long ll = floor(gps.location.lat() + gps.location.lng());
          writeString(ll, 'C');
          Serial.print(gps.location.lat(), 6);
          Serial.print(gps.location.lng(), 6);
          Serial.println(" ");
          isLatLongValid = true;
          delay(1000);
        }
        if (gps.time.isValid()) {
          gotGPSTime = true;
          if (gps.time.hour() < 10) Serial.print(F("0"));
          Serial.print(gps.time.hour());
          Serial.print(F(":"));
          if (gps.time.minute() < 10) Serial.print(F("0"));
          Serial.print(gps.time.minute());
          //Serial.print(F(":"));
          /*if (gps.time.second() < 10) Serial.print(F("0"));
          Serial.print(gps.time.second());
          Serial.print(F("."));
          if (gps.time.centisecond() < 10) Serial.print(F("0"));
          Serial.print(gps.time.centisecond());*/
          Serial.println(" ");
          long hour = gps.time.hour();
          if (!isLatLongValid) {
            hour += tzOffset;
          }
          long time = (hour * 100) + gps.time.minute();
          writeString(time, 'd');
        }
    }
    avail = ss.available();
  }
  delay(1000);
  long gps = (isLatLongValid ? 100 : 0) + (gotGPSTime ? 5 : 0);
  writeString(gps, 'X');
  delay(500);
}

