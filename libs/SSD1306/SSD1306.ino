// OneWire - Version: Latest
#include "sdd1306_config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

void setup_SDD1306() {

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  delay(500);

  // Clear the buffer.
  display.clearDisplay();

  // setup text size
  display.setTextSize(1);

  // monochrome 
  display.setTextColor(WHITE);
}

void clear() {

  // clear display from last write
  display.clearDisplay();
}

void rotate(int i) {
  display.setRotation(i);
}

void write_display(int left, int top, char *buff) {

  display.setCursor(left, top);

  display.print(buff);
}

void show() {
  display.display();
}

/*
void setup() {
  // put your setup code here, to run once:
  setup_SDD1306();
}

void loop() {
  // put your main code here, to run repeatedly:

}*/

