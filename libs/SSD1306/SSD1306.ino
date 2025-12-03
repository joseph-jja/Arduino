// OneWire - Version: Latest
#include "sdd1306_config.h'

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup_SDD1306() {

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.display();
  
  // Clear the buffer.
  display.clearDisplay();

  // setup text size
  display.setTextSize(2);

  // text color
  display.setTextColor(WHITE);
}

void clear_display() {

  // clear display from last write
  display.clearDisplay();
}

void write_display(int left, int top, char *buff) {

  display.setCursor(left, top);

  display.println(buff);

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

