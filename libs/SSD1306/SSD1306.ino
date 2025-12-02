// OneWire - Version: Latest
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#include <stdlib.h>
#include <string.h>

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup_SDD1306() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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

Adafruit_SSD1306 getDisplay() {
  return display;
}

/**/
void setup() {
  // put your setup code here, to run once:
  setup_SDD1306();
}

void loop() {
  // put your main code here, to run repeatedly:

}
*/
