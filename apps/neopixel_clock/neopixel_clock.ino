// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>

static int RED[3] = {255, 0, 0}; 
static int GREEN[3] = {0, 255, 0}; 
static int BLUE[3] = {0, 0, 255}; 

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

void setSinglePixelColor(Adafruit_NeoPixel pixels, int color[], int pixel_index) {

  pixels.setPixelColor(pixel_index, color[0], color[1], color[2]);
}

void loop() {

  
}
