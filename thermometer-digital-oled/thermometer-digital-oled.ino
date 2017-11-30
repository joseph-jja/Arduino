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

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
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
  // for ds18b20
  int Tc_100 = (6 * tempRead) + tempRead / 4;    // multiply by (100 * 0.0625) or 6.25
  //int Tc_100 = (tempRead*100/2);    

  int Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  int Fract = Tc_100 % 100;

  Fract = (Fract < 10 ? 0 : Fract);

  //if (swap) {
    return Whole + (Fract/100);
  //}
    
  //int TemperatureSum = tempRead / 16 - 2;
  //return TemperatureSum;
}

void writeTemps(float c1, float f1) {
  
  char buff[24];

  // clear display from last write
  display.clearDisplay();

  // setup text size
  display.setTextSize(2);

  // text color
  display.setTextColor(WHITE);

  int celciusLeft = 5,
    fahrenheitLeft = 70;
  
  if (swap) {
    celciusLeft = 70;
    fahrenheitLeft = 5;
    swap = false;
  } else {
    swap = true;
  }

  // write data at positions
  display.setCursor(celciusLeft, 10);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%dC", (long)c1);
  display.println(buff);

  display.setCursor(fahrenheitLeft, 10);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%dF", (long)f1);
  display.println(buff);

  // display data
  display.display();
  delay( 1000 );
}

void loop()
{
  float celsius1 = getCTemp(pinTwo);
  float fahrenheit1 = (celsius1 * 1.8) + 32.0;

  writeTemps(celsius1, fahrenheit1);
  delay( 1000 );
}

