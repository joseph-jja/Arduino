
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
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();

  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();

  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();

  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  
  ss.begin(GPSBaud);

  delay(250);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDisplay();

  Serial.begin(9600);
  Serial.println("Application setup!");
  Serial.println(TinyGPSPlus::libraryVersion());
}

// method to write a float to screen
void writeString(long value)
{
  int i;
  int len;
  char data[(sizeof(long) * 4) + 1];

  memset(&data, '\0', sizeof(data));
  sprintf(data, "%4d", value);
  len = strlen(data);

  alpha4.clear();
  alpha4.writeDisplay();

  i = 0;

  // 100 i will be 0, 1, then 2
  // 80 i will be 0, 1
  while (i < 4)
  {
    if ( i >= len )
    {
      break;
    }
    alpha4.writeDigitAscii(i, data[i]);
    i++;
  }
  alpha4.writeDisplay();
  delay(1000);
}

void loop()
{

  char buff[255];
  
  bool newData = false;

  long avail = ss.available();
  memset(&buff, '\0', sizeof(buff));
  sprintf(buff, "Start %d ", avail);
  Serial.println(buff);
  delay(2000);
  memset(&buff, '\0', sizeof(buff));
  sprintf(buff, "Read %d ", ss.read());
  Serial.println(buff);
  delay(2000);
  
  delay(2000);
  writeString(avail);
  // This sketch displays information every time a new sentence is correctly encoded.
  /*while (ss.available() > 0) {
    Serial.println("here");
    if (gps.encode(ss.read()))
      {
        memset(&buff, '\0', sizeof(buff));
        //sprintf(buff, "Start %d %d", ss.available());
        //Serial.println(buff);
        if (gps.location.isValid())
        {
          Serial.print(gps.location.lat(), 6);
          Serial.print(F(","));
          Serial.print(gps.location.lng(), 6);
        }
        else
        {
          Serial.print(F("INVALID"));
        }
      }
  }*/

  /*if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/
}

