// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

// OneWire - Version: Latest
#include <OneWire.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <stdlib.h>
#include <string.h>

// define the address of the DS18*20 sensors we know about
#define DS18S20      0x10
#define DS18B20      0x28

// DS18B20 pin
#define TEMPERATURE_PIN 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(TEMPERATURE_PIN);

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

boolean debugMode = false;

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

   Serial.begin(9600);;
  delay(2000);
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
    default:
      alpha4.writeDigitRaw(1, 0x4000);
      break;
  }
  alpha4.writeDisplay();
}

// method to write a float to screen
void writeString(long value, char type, long writeDot)
{
  int i;
  int len;
  char data[(sizeof(long) * 3) + 1];

  memset(&data, '\0', sizeof(data));
  snprintf(data, sizeof(data), "%3d", value);
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
  if ( writeDot > 0 ) {
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
  if ( debugMode ) { 
    Serial.print("  Temperature = ");
    Serial.print(celsius);
    Serial.print(" Celsius, ");
    Serial.print(fahrenheit);
    Serial.println(" Fahrenheit");
 }
  delay(1000);

  writeString( celsius*10, 'C', 1L );
  delay( 2500 );
  writeString( fahrenheit*10, 'F', 1L );
  delay( 2500 );
}
