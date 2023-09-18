
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
  
  delay(250);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDisplay();

  Serial.begin(GPSBaud);
  Serial.println("Application setup!");
  Serial.println(TinyGPSPlus::libraryVersion());

  ss.begin(GPSBaud);
}

// method to write a float to screen
void writeString(long value, char type)
{
  int i;
  int len;
  char data[(sizeof(long) * 4) + 1];

  memset(&data, '\0', sizeof(data));
  sprintf(data, "%c%3d", type, value);
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
  writeString(avail, 'A');

  while (avail > 0) {
    //Serial.println("Trying to get GPS data obtained");
    if (gps.encode(ss.read())) {
        Serial.println("GPS data obtained");
        writeString(100, 'B');
        //if (gps.location.isValid()) {
          memset(&buff, '\0', sizeof(buff));
          sprintf(buff, "Latitude: %6d \t Longitude: %6d ", gps.location.lat(), gps.location.lng()); 
          Serial.println(buff);
          //Serial.println("Latitude: %6d \t Longitude: %6d");
          //Serial.print(gps.location.lat(), 6);
          //Serial.print(F(","));
          //Serial.print(gps.location.lng(), 6);
        //}
        if (gps.location.isValid()) {
          writeString(300, 'C');
          delay(2000);
        }
        if (gps.time.isValid()) {
          if (gps.time.hour() < 10) Serial.print(F("0"));
          Serial.print(gps.time.hour());
          Serial.print(F(":"));
          if (gps.time.minute() < 10) Serial.print(F("0"));
          Serial.print(gps.time.minute());
          Serial.print(F(":"));
          if (gps.time.second() < 10) Serial.print(F("0"));
          Serial.print(gps.time.second());
          Serial.print(F("."));
          if (gps.time.centisecond() < 10) Serial.print(F("0"));
          Serial.print(gps.time.centisecond());
        }
    }
    avail = ss.available();
  }

  /*if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/
  delay(5000);
}

