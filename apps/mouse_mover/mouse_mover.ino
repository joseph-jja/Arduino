#include "USB.h"
#include "USBHIDMouse.h"

// Initialize the USB Mouse object
USBHIDMouse Mouse;

void setup() {

  Serial.begin(9600);
  Serial.println("Starting Mouse Device");

  // Begin the USB HID service
  Mouse.begin();
  USB.begin();
}

void loop() {


  Serial.println("Trying to Move");
  // Move mouse in a square pattern
  Mouse.move(100, 0);  // Right 10
  delay(500);
  Mouse.move(0, 100);  // Down 10
  delay(500);
  Mouse.move(-100, 0);  // Left 10
  delay(500);
  Mouse.move(0, -100);  // Up 10
  delay(500);

  // Example: Click left button
  // Mouse.click(MOUSE_LEFT);
}
