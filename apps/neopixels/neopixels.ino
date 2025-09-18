// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>

// Trinket, Gemma, etc.
#ifdef __AVR_ATtiny85__
#include <avr/power.h>
#endif

#define PIN_TOP 9
#define PIN_LEFT 10
#define PIN_B_LEFT 11
#define PIN_B_RIGHT 12
#define PIN_RIGHT 13

#define PIXEL_CT 5

Adafruit_NeoPixel pixels_top = Adafruit_NeoPixel(PIXEL_CT, PIN_TOP);
Adafruit_NeoPixel pixels_left = Adafruit_NeoPixel(PIXEL_CT, PIN_LEFT);
Adafruit_NeoPixel pixels_b_left = Adafruit_NeoPixel(PIXEL_CT, PIN_B_LEFT);
Adafruit_NeoPixel pixels_b_right = Adafruit_NeoPixel(PIXEL_CT, PIN_B_RIGHT);
Adafruit_NeoPixel pixels_right = Adafruit_NeoPixel(PIXEL_CT, PIN_RIGHT);

void setup() {

  pixels_top.begin();
  pixels_top.setBrightness(85); // 1/3 brightness
  pixels_top.show();

  pixels_left.begin();
  pixels_left.setBrightness(85); // 1/3 brightness
  pixels_left.show();

  pixels_b_left.begin();
  pixels_b_left.setBrightness(85); // 1/3 brightness
  pixels_b_left.show();

  pixels_b_right.begin();
  pixels_b_right.setBrightness(85); // 1/3 brightness
  pixels_b_right.show();

  pixels_right.begin();
  pixels_right.setBrightness(85); // 1/3 brightness
  pixels_right.show();

}

void setSinglePixelColor(Adafruit_NeoPixel pixels, int i, int cycle) {

  int red = 255,
      green = 0,
      blue = 0;

  switch (cycle) {
    // red
    case 0:
      red = 255;
      green = 0;
      blue = 0;
      break;
    // orange
    case 1:
      red = 255;
      green = 165;
      blue = 0;
      break;
    // yellow
    case 2:
      red = 255;
      green = 255;
      blue = 0;
      break;
    // green
    case 3:
      red = 0;
      green = 128;
      blue = 0;
      break;
    // blue
    case 4:
      red = 0;
      green = 0;
      blue = 255;
      break;
    // purple
    case 5:
    default:
      red = 128;
      green = 0;
      blue = 128;
      break;
  }

  pixels.setPixelColor(i, red, green, blue);
}

int xcycle = 0;

void loop() {
  int i,
      j = 0;

  for (i = 0; i < PIXEL_CT; i++) {
    // red 255, 0, 0
    // green 0, 255, 0
    // blue 0, 0, 255
    int x = (j < 5 ) ? j + 1 : 0, 
      y = (x < 5 ) ? x + 1 : 0, 
      z = (y < 5 ) ? y + 1 : 0, 
      a = (z < 5 ) ? z + 1 : 0;
    setSinglePixelColor(pixels_top, i, ((j + xcycle) % 6));
    setSinglePixelColor(pixels_left, i, ((x + xcycle) % 6));
    setSinglePixelColor(pixels_b_left, i, ((y + xcycle) % 6));
    setSinglePixelColor(pixels_b_right, i, ((z + xcycle) % 6));
    setSinglePixelColor(pixels_right, i, ((a + xcycle) % 6));
    j++;
    if ( j > 6 ) {
      j = 0;
    }
    delay(250);
  }

  pixels_top.show();
  delay(250);
  xcycle++;

}
