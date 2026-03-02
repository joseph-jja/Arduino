#include "USB.h"
#include "USBHIDMouse.h"

// Initialize the USB Mouse object
USBHIDMouse Mouse;

/*
#define RGB_BRIGHTNESS 64 // Limit brightness
neopixelWrite(48, RGB_BRIGHTNESS, 0, 0); // Set to Red
*/
void setup() {

  Serial.begin(9600);
  Serial.println("Starting Mouse Device");

  // Begin the USB HID service
  Mouse.begin();
  USB.begin();

  randomSeed(analogRead(A0));
}

void loop() {

  long randNumber = random(600);
  long negRandNumber = randNumber * -1;


  Serial.println("Trying to Move");
  // Move mouse in a square pattern
  Mouse.move(randNumber, 0);  // Right 
  delay(500);
  Mouse.move(0, randNumber);  // Down
  delay(500);
  Mouse.move(negRandNumber, 0);  // Left
  delay(500);
  Mouse.move(0, negRandNumber);  // Up 
  delay(500);

  // Example: Click left button
  // Mouse.click(MOUSE_LEFT);
}
