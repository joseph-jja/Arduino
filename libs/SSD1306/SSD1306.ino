// OneWire - Version: Latest
#include "sdd1306_config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

bool found = false;

void setup_SDD1306() {

  found = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  if (found) {
    Serial.println("Found display!");
  } else {
    Serial.println("No display was found!");
    return;
  }
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

  if (!found) {
    return;
  }

  // clear display from last write
  display.clearDisplay();
}

void rotate(int i) {
  if (!found) {
    return;
  }
  display.setRotation(i);
}

void write_display(int left, int top, const char buff[]) {

  if (!found) {
    return;
  }

  display.setCursor(left, top);

  display.print(buff);
}

void write_number(int left, int top, double x) {

  char buff[32];
  memset(buff, '\0', 32);
  sprintf(buff, "%0.02f", x);
  write_display(left, top, buff);
}

void show() {
  if (!found) {
    return;
  }
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

