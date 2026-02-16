// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>

#define HOURS_ONE 18
#define HOURS_TWO 17
#define MINUTES_ONE 16
#define MINUTES_TWO 15
#define SECONDS_ONE 7
#define SECONDS_TWO 6


#define PIXEL_CT 4

Adafruit_NeoPixel pixels_hours_1 = Adafruit_NeoPixel(PIXEL_CT, HOURS_ONE);
Adafruit_NeoPixel pixels_hours_2 = Adafruit_NeoPixel(PIXEL_CT, HOURS_TWO);
Adafruit_NeoPixel pixels_minutes_1 = Adafruit_NeoPixel(PIXEL_CT, MINUTES_ONE);
Adafruit_NeoPixel pixels_minutes_2 = Adafruit_NeoPixel(PIXEL_CT, MINUTES_TWO);
Adafruit_NeoPixel pixels_seconds_1 = Adafruit_NeoPixel(PIXEL_CT, SECONDS_ONE);
Adafruit_NeoPixel pixels_seconds_2 = Adafruit_NeoPixel(PIXEL_CT, SECONDS_TWO);

void setup() {

  pixels_hours_1.begin();
  pixels_hours_1.setBrightness(85); // 1/3 brightness
  pixels_hours_1.show();

  pixels_hours_2.begin();
  pixels_hours_2.setBrightness(85); // 1/3 brightness
  pixels_hours_2.show();

  pixels_minutes_1.begin();
  pixels_minutes_1.setBrightness(85); // 1/3 brightness
  pixels_minutes_1.show();

  pixels_minutes_2.begin();
  pixels_minutes_2.setBrightness(85); // 1/3 brightness
  pixels_minutes_2.show();

  pixels_seconds_1.begin();
  pixels_seconds_1.setBrightness(85); // 1/3 brightness
  pixels_seconds_1.show();

  pixels_seconds_2.begin();
  pixels_seconds_2.setBrightness(85); // 1/3 brightness
  pixels_seconds_2.show();
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
