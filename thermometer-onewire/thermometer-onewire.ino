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
  sprintf(data, "%3d", value);
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

float getCTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  oneWire.reset_search();
  if ( !oneWire.search(addr)) {
      //no more sensors on chain, reset search
      if ( debugMode ) { 
        Serial.println("No sensors on the chain!");
      }
      oneWire.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      if ( debugMode ) { 
        Serial.println("CRC is not valid!");
      }
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      if ( debugMode ) { 
        Serial.print("Device is not recognized");
      }
      return -1000;
  }

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44,0); // start conversion, with parasite power off at the end

  // I read we need a delay?
  delay(750);
  
  byte present = oneWire.reset();
  oneWire.select(addr);    
  oneWire.write(0xBE); // Read Scratchpad

  delay(750);
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = oneWire.read();
  }

  oneWire.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16 - 2;

  if ( debugMode ) { 
    Serial.print("  Raw = ");
   Serial.println( TemperatureSum);
  }
  return TemperatureSum;
}

void loop()
{
  float celsius = getCTemp();
  float fahrenheit = celsius * 1.8 + 32.0;
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

