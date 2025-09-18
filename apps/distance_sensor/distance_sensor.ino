#include <TinyWireM.h>
//#include <avr/power.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define EZ1pin 1               // Trinket GPIO #1   

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4(); 
char printMe[3];
String str;
int8_t arraysize = 9; // quantity of values to find the median (sample size). Needs to be an odd number
uint16_t rangevalue[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t modE;        // calculated median distance

void setup() {
  pinMode(EZ1pin, INPUT); // Sey ultrasonic sensor pin as input
  
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  alpha4.begin(0x70);  // pass in the address
  alpha4.writeDisplay();  // clear display
}

void loop() {
  int16_t pulse;  // number of pulses from sensor
  int i=0;
  
  while( i < arraysize )
  {                    
    pulse = pulseIn(EZ1pin, HIGH);  // read in time for pin to transition
    rangevalue[i]=pulse/58;         // pulses to centimeters (use 147 for inches)
    if( rangevalue[i] < 645 && rangevalue[i] >= 15 ) i++;  // ensure no values out of range
    delay(10);                      // wait between samples
  }
  isort(rangevalue,arraysize);        // sort samples
  modE = mode(rangevalue,arraysize);  // get median
  str=String(modE);
  str.toCharArray(printMe,3);


    alpha4.writeDigitAscii(0, printMe[0]);
    alpha4.writeDigitAscii(1, printMe[1]);
    alpha4.writeDigitAscii(2, 'C');   // write to LCD
    alpha4.writeDigitAscii(3, 'M');
    alpha4.writeDisplay();
  delay(500);                         // Read every half second
}

// Sorting function (Author: Bill Gentles, Nov. 12, 2010)
void isort(uint16_t *a, int8_t n){
  for (int i = 1; i < n; ++i)  {
    uint16_t j = a[i];
    int k;
    for (k = i - 1; (k >= 0) && (j < a[k]); k--) {
      a[k + 1] = a[k];
    }
    a[k + 1] = j;
  }
}

// Mode function, returning the mode or median.
uint16_t mode(uint16_t *x,int n){
  int i = 0;
  int count = 0;
  int maxCount = 0;
  uint16_t mode = 0;
  int bimodal;
  int prevCount = 0;
  while(i<(n-1)){
    prevCount=count;
    count=0;
    while( x[i]==x[i+1] ) {
      count++;
      i++;
    }
    if( count > prevCount && count > maxCount) {
      mode=x[i];
      maxCount=count;
      bimodal=0;
    }
    if( count == 0 ) {
      i++;
    }
    if( count == maxCount ) {      //If the dataset has 2 or more modes.
      bimodal=1;
    }
    if( mode==0 || bimodal==1 ) {  // Return the median if there is no mode.
      mode=x[(n/2)];
    }
  }
  return mode;
}
