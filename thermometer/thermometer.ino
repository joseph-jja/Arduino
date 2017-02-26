  // For use with Gemma or Trinket (Attiny85) - requires TinyWireM
// https://github.com/adafruit/TinyWireM

// this for Attiny85 based like Trinket & Gemma
#include "TinyWireM.h"

// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <stdlib.h>
#include <string.h>

//TMP36 Pin Variables
//the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
int sensorPin = 2;
int togglePin = 3;

long subtractor = 5000L;

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void setup()
{
  // it does not seem to work unless you pass in the address?
  alpha4.begin(0x70);

  // this turns off 3, then on 1
  // then does off 1 & on 2, off 2 & on 3, off 3 & on 4
  //alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  //alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  //alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);
  //alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);

  delay(2000);
  // not clearing all make em wiat!
  // clear all
  //alpha4.clear();
  //alpha4.writeDisplay();
  
  pinMode( togglePin, INPUT );
}

void writeDotToDisplay(long val) {

  switch (val) {
      case 0:
        alpha4.writeDigitRaw(1, 0x4C3F);
        break; 
      case 1:
        alpha4.writeDigitRaw(1, 0x4006);
        break; 
      case 2:
        alpha4.writeDigitRaw(1, 0x40DB);
        break; 
      case 3:
        alpha4.writeDigitRaw(1, 0x40CF);
        break; 
      case 4:
        alpha4.writeDigitRaw(1, 0x40E6);
        break; 
      case 5:
        alpha4.writeDigitRaw(1, 0x40ED);
        break; 
      case 6:
        alpha4.writeDigitRaw(1, 0x40FD);
        break; 
      case 7:
        alpha4.writeDigitRaw(1, 0x4007);
        break;      
      case 8:
        alpha4.writeDigitRaw(1, 0x40FF);
        break;      
      case 9:
        alpha4.writeDigitRaw(1, 0x40EF);
        break;      
     }
  alpha4.writeDisplay();
}

// method to write a float to screen
void writeString(long value, char type, long writeDot)
{
  int i;
  int len;
  char data[64];
  boolean doWriteDot = (writeDot > 0);

  memset(&data, '\0', sizeof(data));
  ltoa(value, data, 10);
  len = strlen(data);

  alpha4.clear();
  alpha4.writeDisplay();

  // last character will be the measurement type
  // R = Analog Read
  // V = voltage
  // C = Celcius
  // F = Fahrenheit
  alpha4.writeDigitAscii(3, type);

  i = 0;
  
  // we need to handle 0 
  if ( value === 0L ) { 
    data[0] = ' ';
    data[1] = '0';
    data[2] = ' ';
    data[3] = '\0';
  }
  
  // if we get 7 or 4 then it should be 7.0
  if ( len < 2 ) {
    
  }
  
  // now handle 72 or 70 which would be 7.2
  if ( len < 3 ) {
    alpha4.writeDigitAscii(0, ' ');
    i = 1;
  }
  
  // 100 i will be 0, 1, then 2
  // 80 i will be 0, 1
  while (i < 3)
  {
    if ( i >= len )
    {
      break;
    }
    alpha4.writeDigitAscii(i, data[i]);
    i++;
  }

  // all this code to write a decimal place display 
  if ( doWriteDot ) { 
    // https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric
    // DP N M L K J H G2 G1 F E D C B A
    // 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 = dot or 0x4000 or 16384
    char buf[1]; 
    memset(&buf, '\0', sizeof(buf));
    buf[0] = data[1];

    long val = atol(buf);
    writeDotToDisplay(val);
  }
  alpha4.writeDisplay();
  delay(1000);    
}

void loop()
{
  //getting the voltage reading from the temperature sensor
  long reading = (long)analogRead( sensorPin );
  // wait a little before trying to read the digital pin
  delay(500);
  
  boolean isHigh = digitalRead(togglePin);
  if ( isHigh ) {
    writeString( reading, 'R', 0L );
    delay( 1000 );
  }

  // converting that reading to voltage, for 3.3v arduino use 3.3
  // so the real voltage at my pin is 3.25 volts but this does not give me the correct voltage
  // analogRead return 156 so
  // 156 * 330 / 1024 = 500
  // making everything in milivolts and remove floats
  // because we are using longs we need to multiply by 1000 and divide by 1000 for better accuracy
  long voltage = (long)( ( reading * ( ( 3375L * 1000L ) / 1024L ) ) / 100L );
  if ( isHigh ) {
    // using pin 3 we can now see the voltage if the pin goes high
    writeString( voltage / 10L, 'V', 0L );
    delay( 2000 );
  }

  // now print out the temperature (16.7)
  //converting from 10 mv per degree with 500 mV offset
  // I'm using 430 as I have a lower voltage?
  long temperatureC = (long)( ( voltage - subtractor ) / 10L );
  writeString( temperatureC, 'C', 1L );
  delay( 2500 );

  // now convert to Fahrenheit (62)
  long temperatureF = (long)( ( ( ( ( temperatureC / 10 ) * 9 ) / 5 ) + 32 ) * 10);
  if ( temperatureF >= 1000 ) { 
    writeString( temperatureF, 'F', 0L );
  } else {
    writeString( temperatureF, 'F', 1L );
  }
  delay( 2500 );
}

