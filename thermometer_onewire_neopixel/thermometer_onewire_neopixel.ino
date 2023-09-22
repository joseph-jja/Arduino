// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

// neopixel
#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

// OneWire - Version: Latest
#include <OneWire.h>

#include <stdlib.h>
#include <string.h>

// define the address of the DS18*20 sensors we know about
#define DS18S20      0x10
#define DS18B20      0x28

// DS18B20 pin
#define TEMPERATURE_PIN 10

#define PIXELS_PIN 6
#define NUMPIXELS 20

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(TEMPERATURE_PIN);

Adafruit_DotStar strip(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
//Adafruit_NeoPixel pixels(NUMPIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);

boolean debugMode = false;

void setup()
{ 
  strip.begin(); // Initialize pins for output
  strip.setBrightness(50);
  strip.show(); 

  /* basic startup test to make sure pixels are all lighting up
  pixels.begin(); // Initialize pins for output
  pixels.setBrightness(50);
  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }

  pixels.show();
  delay(500);*/
}

float getCTemp(OneWire wire){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  wire.reset_search();
  if ( !wire.search(addr)) {
      //no more sensors on chain, reset search
      wire.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      return -2000;
  }

  if ( addr[0] != DS18S20 && addr[0] != DS18B20) {
      return -3000;
  }

  //wire.reset();
  //wire.select(addr);
  //wire.write(0x4E);
  //wire.write(0x00);
  //wire.write(0x00);
  //wire.write(0x7F);

  // I read we need a delay?
  //delay(750);

  //wire.reset(); // reset 1-Wire
  //wire.select(addr); // select DS18B20
  //wire.write(0x48); // copy scratchpad to l’EE

  // I read we need a delay?
  //delay(750);

  wire.reset();
  wire.select(addr);
  wire.write(0x44,0); // start conversion, with parasite power off at the end

  // I read we need a delay?
  delay(750);
  
  byte present = wire.reset();
  wire.select(addr);    
  wire.write(0xBE); // Read Scratchpad

  delay(750);

  // we have resolution accurate to +/-.5C
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = wire.read();
  }

  wire.reset_search();

  byte LSB = data[0];
  byte MSB = data[1];

  int tempRead = ((MSB << 8) | LSB); //using two's compliment

  int SignBit = tempRead & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    tempRead = (tempRead ^ 0xffff) + 1; // 2's comp
  }
  
  // after reading the sensor we will convert it
  // to the temperature in celcius times 100
  // this way it stays an int
  int tcTimes100 = -4000;    

  // for ds18b20 
  if (addr[0] == DS18B20) { 
    // DS18B20 0.0625 deg resolution 
    // multiply by (100 * 0.0625) or 6.25 or 6 + 1/4
    tcTimes100 = (6 * tempRead) + tempRead / 4; 
  } else if (addr[0] == DS18S20) {
    // DS18S20 sensor has .05 resolution
    // this is the same as dividing by 2 and multiplying by 10
    // to get 1920 value
    tcTimes100 = (tempRead / 2) * 10; 
  } else {
    // can't remember where this came from?
    // but I thimk it ends up being the same 0.625 resolution
    tcTimes100 = (tempRead / 16 - 2) * 100;
  }

  // at this point
  // tempRead will now be 2200 for 22.00C or 2268 for 22.68C
  
  // separate off the whole and fractional portions
  int whole = tcTimes100 / 100;  
  int fract = tcTimes100 % 100;
  
  // we return a float
  return (whole + (fract/100));
}

void loop()
{
  // we compensate for our sensor looks to be +0.50C high? 
  // it seems most of these sensors have +/- tollerance
  float celsius = getCTemp(oneWire) - 0.50;
  float fahrenheit = (celsius * 1.80) + 32.00;

  // clear all pixels colors 
  // for ( int i=0; i < len; i++ ) {
  //  xx.setPixelColor(0, 0, 0, 0);  
  //}

  if (fahrenheit < 10) {
    strip.setPixelColor(0, 255, 255, 0);
    // set all led this colod
  } else if (fahrenheit > 100) {
    strip.setPixelColor(0, 255, 0, 0);
    // set all led this colod
  } else {
    strip.setPixelColor(0, 0, 0, 255);    

    long hundreds = floor(fahrenheit/10);
    /*
    pixels.setPixelColor(hundreds, 0, 0, 255);
    if (tenx < 9 ) {
      pixels.setPixelColor(hundreds+1, 0, 255, 0);
    } else if (tens > 0) {
      pixels.setPixelColor(hundreds-1, 255, 0, 255);
    }

    long tens = fahrenheit - (hundreds * 10);
    pixels.setPixelColor(tens, 0, 0, 255);
    if (tenx < 9 ) {
      pixels.setPixelColor(tens+1, 0, 255, 0);
    } else if (tens > 0) {
      pixels.setPixelColor(tens=-1, 255, 0, 255);
    }
    */ 
  }
  strip.show();

  delay(1000);
}
