// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

// neopixel
#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

// OneWire - Version: Latest
#include <DS18B20.h>

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
DS18B20 ds(TEMPERATURE_PIN);

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

void loop()
{

  float celsius;
  float fahrenheit;
  if (ds.selectNext()) {
    celcius = ds.getTempC();
    fahrenheit = ds.getTempC();
  }

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
