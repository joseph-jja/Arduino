// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

// OneWire - Version: Latest 
#include <OneWire.h>

// DallasTemperature - Version: Latest 
#include <DallasTemperature.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <stdlib.h>
#include <string.h>

// DS18B20 pin
#define TEMPERATURE_PIN 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(TEMPERATURE_PIN);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

DeviceAddress insideThermometer;

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

  sensors.begin();

 if ( debugMode ) { 
   Serial.begin(9600);

   Serial.print("Dallas Temperature Sensor - DS18B20. Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" sensors.");

    if (!sensors.getAddress(insideThermometer, 0)) {
      Serial.println("Unable to find address for Device 0"); 
    }
    
    Serial.print("Device 0 Resolution: ");
    Serial.print(sensors.getResolution(insideThermometer), DEC); 
    Serial.println();
  }
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
  char data[(sizeof(long)*3)+1];

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


void loop()
{
  // get temperatures
  sensors.requestTemperatures();
   
  // now print out the temperature (16.7)
  //converting from 10 mv per degree with 500 mV offset
  // I'm using 430 as I have a lower voltage?
  long temperatureC = (long)(sensors.getTempCByIndex(0) * 10);
  if ( debugMode ) {
    Serial.println( "C temperature" );
    Serial.println( temperatureC, DEC );
  }
  writeString( temperatureC, 'C', 1L );
  delay( 2500 );

  // now convert to Fahrenheit (62)
  long temperatureF = (long)( ( ( ( ( temperatureC / 10 ) * 9 ) / 5 ) + 32 ) * 10);
  if ( debugMode ) {
      Serial.println( "F temperature" );
      Serial.println( temperatureF );
  }
  if ( temperatureF >= 1000 ) { 
    writeString( temperatureF, 'F', 0L );
  } else {
    writeString( temperatureF, 'F', 1L );
  }
  delay( 2500 );
}
