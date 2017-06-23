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

// DS18B20 pin
#define TEMPERATURE_PIN_ONE 10
#define TEMPERATURE_PIN_TWO 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire pinOne(TEMPERATURE_PIN_ONE);
OneWire pinTwo(TEMPERATURE_PIN_TWO);

boolean debugMode = true;

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
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      return -1000;
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

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16 - 2;

  return TemperatureSum;
}

void writeTemps(float c1, float c2, float f1, float f2) {
  
  char buff[24];

  // clear display from last write
  display.clearDisplay();

  // setup text size
  display.setTextSize(1);

  // text color
  display.setTextColor(WHITE);

  // write data at positions
  display.setCursor(0,0);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "Tank   C      F");
  display.println(buff);
  
  display.setCursor(0,12);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "1     %d", (long)c1);
  display.println(buff);

  display.setCursor(78,12);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%d", (long)f1);
  display.println(buff);

  display.setCursor(0,24);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "1     %d", (long)c2);
  display.println(buff);

  display.setCursor(78,24);
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "%d", (long)f2);
  display.println(buff);

  // display data
  display.display();
  delay(1);
}

void loop()
{
  float celsius1 = getCTemp(pinOne);
  float fahrenheit1 = (celsius1 * 1.8) + 32.0;
  delay(1000);

  float celsius2 = getCTemp(pinTwo);
  float fahrenheit2 = (celsius2 * 1.8) + 32.0;

  writeTemps(celsius1, celsius2, fahrenheit1, fahrenheit2);
  delay( 2500 );
}

