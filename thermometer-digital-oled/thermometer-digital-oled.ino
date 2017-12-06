// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

// OneWire - Version: Latest
#include <OneWire.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#include <stdlib.h>
#include <string.h>

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// define the address of the DS18*20 sensors we know about
#define DS18S20      0x10
#define DS18B20      0x28

// DS18B20 pin
#define TEMPERATURE_PIN_TWO 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire pinTwo(TEMPERATURE_PIN_TWO);

boolean swap = false;

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  
  // Clear the buffer.
  display.clearDisplay();
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

  wire.reset();
  wire.select(addr);
  wire.write(0x44,0); // start conversion, with parasite power off at the end

  // I read we need a delay?
  delay(750);
  
  byte present = wire.reset();
  wire.select(addr);    
  wire.write(0xBE); // Read Scratchpad

  delay(750);
  
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

// so I could probably include math.h and use round from there
// I wanted something slightly different  
// but I find if I don't need the lib why use it :) 
int getFractionPart(float inputValue) {

  int inValTimes100 = inputValue * 100;
  int fract = inValTimes100 % 100;
  int fractRound = fract % 10;
    
  // if the fractional part is greater than 50 we add 1 to the whole number
  if ( fractRound >= 5 && fract < 90 ) {
    fract += 10;
  }
  
  return (fract / 10); 
}

void writeTemps(float c1, float f1) {
  
  char buff[24];

  // clear display from last write
  display.clearDisplay();

  // setup text size
  display.setTextSize(2);

  // text color
  display.setTextColor(WHITE);

  int celciusLeft = 10,
    fahrenheitLeft = 50, 
    top = 10, 
    bottom = 30;
  
  if (swap) {
    celciusLeft = 50;
    fahrenheitLeft = 10;
    swap = false;
  } else {
    swap = true;
  }
  
  // write data at positions
  display.setCursor(celciusLeft, top);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%d.%dC", (int)c1, getFractionPart(c1));
  display.println(buff);

  display.setCursor(fahrenheitLeft, bottom);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%d.%dF", (int)f1, getFractionPart(f1));
  display.println(buff);

  // display data
  display.display();
  delay( 1000 );
}

void loop()
{
  float celsius1 = getCTemp(pinTwo);
  float fahrenheit1 = (celsius1 * 1.80) + 32.00;

  writeTemps(celsius1, fahrenheit1);
  delay( 1000 );
}

