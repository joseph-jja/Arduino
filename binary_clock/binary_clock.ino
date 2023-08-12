#include <string.h>

#include <time.h>

#include <Adafruit_NeoPixel.h>

#define    PIXEL_COUNT     24

#define    PIXEL_PIN       4

#define    COLOR_COUNT     10

static int RED[3]        = {255, 0, 0};
static int LIGHT_RED[3]  = {128, 0, 0};
static int ORANGE[3]     = {255, 165, 0};
static int YELLOW[3]     = {255, 255, 0};
static int GREEN[3]      = {0, 255, 0};
static int BLUE[3]       = {0, 0, 255};
static int LIGHT_BLUE[3] = {0, 0, 128};
static int PURPLE[3]     = {128, 0, 128};
static int WHITE[3]      = {255, 255, 255};
static int BLACK[3]      = {0, 0, 0};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN);

int timeBlock[PIXEL_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void decimal2binary(int i, int result[]) {

    switch (i) {
        case 0:
            result[0] = 0;
            result [1] = 0;
            result[2] = 0;
            result[3] = 0;
            break;
        case 1:
            result[0] = 0;
            result [1] = 0;
            result[2] = 0;
            result[3] = 1;
            break;
        case 2:
            result[0] = 0;
            result [1] = 0;
            result[2] = 1;
            result[3] = 0;
            break;
        case 3:
            result[0] = 0;
            result [1] = 0;
            result[2] = 1;
            result[3] = 1;
            break;
        case 4:
            result[0] = 0;
            result [1] = 1;
            result[2] = 0;
            result[3] = 0;
            break;
        case 5:
            result[0] = 0;
            result [1] = 1;
            result[2] = 0;
            result[3] = 1;
            break;
        case 6:
            result[0] = 0;
            result [1] = 1;
            result[2] = 1;
            result[3] = 0;
            break;
        case 7:
            result[0] = 0;
            result [1] = 1;
            result[2] = 1;
            result[3] = 1;
            break;
        case 8:
            result[0] = 1;
            result [1] = 0;
            result[2] = 0;
            result[3] = 0;
            break;
        case 9:
            result[0] = 1;
            result [1] = 0;
            result[2] = 0;
            result[3] = 1;
            break;
    }
}

void setTimepart(int a, int b, int c, int d,
    int e, int f, int g, int h, int timeIn) {

    int tens = floor(timeIn/10);
    int remainder = timeIn - (tens*10);

    int tensx[4],
       secx[4];
    decimal2binary(tens, tensx);
    decimal2binary(remainder, secx);

    timeBlock[a] = secx[0];
    timeBlock[b] = secx[1];
    timeBlock[c] = secx[2];
    timeBlock[d] = secx[3];

    timeBlock[e] = tensx[0];
    timeBlock[f] = tensx[1];
    timeBlock[g] = tensx[2];
    timeBlock[h] = tensx[3];
}

// color from 0 to 10
void get_color(int index, int results[3]) {

    switch(index) {
        case 0:
            results[0] = RED[0];
            results[1] = RED[1];
            results[2] = RED[2];
            break;
        case 1:
            results[0] = LIGHT_RED[0];
            results[1] = LIGHT_RED[1];
            results[2] = LIGHT_RED[2];
            break;
        case 2:
            results[0] = ORANGE[0];
            results[1] = ORANGE[1];
            results[2] = ORANGE[2];
            break;
        case 3:
            results[0] = YELLOW[0];
            results[1] = YELLOW[1];
            results[2] = YELLOW[2];
            break;
        case 4:
            results[0] = GREEN[0];
            results[1] = GREEN[1];
            results[2] = GREEN[2];
            break;
        case 5:
            results[0] = BLUE[0];
            results[1] = BLUE[1];
            results[2] = BLUE[2];
            break;
        case 6:
            results[0] = LIGHT_BLUE[0];
            results[1] = LIGHT_BLUE[1];
            results[2] = LIGHT_BLUE[2];
            break;
        case 7:
            results[0] = PURPLE[0];
            results[1] = PURPLE[1];
            results[2] = PURPLE[2];
            break;
        case 8:
            results[0] = WHITE[0];
            results[1] = WHITE[1];
            results[2] = WHITE[2];
            break;
        case 9:
            results[0] = BLACK[0];
            results[1] = BLACK[1];
            results[2] = BLACK[2];
            break;
    }
}

void setup() {

    pixels.begin();
    pixels.setBrightness(85); // 1/3 brightness
    pixels.show();

    Serial.begin(9600);
}

int ds = 0, sec = 0;

// white, red, light  red,  orange, yellow, green,  light blue, blue,     purple, black
// black, light blue, blue, green,  purple, orange, red,       light red, yellow, white
int colors[COLOR_COUNT]      = {8, 0, 1, 2, 3, 4, 6, 5, 7, 9};
int anti_colors[COLOR_COUNT] = {9, 6, 5, 4, 7, 2, 0, 1, 3, 8};

void get_time(int *ohour, int *omin, int *osec) {

    time_t now = time(NULL);
    const struct tm *localNow = localtime(&now);

    int isec = localNow->tm_sec;
    int imin = localNow->tm_min;
    int ihours = localNow->tm_hour;
    
    isec = (millis() % 60000);

    *osec = isec;
    *omin = imin;
    *ohour = ihours;
    
    Serial.print(isec);
    Serial.print(imin);
    Serial.print(ihours);
}

int change = 0;

void loop() {

    int ohour, omin, osec;

    get_time(&ohour, &omin, &osec);

    ds = ds + 1;
    if (ds >= COLOR_COUNT) {
        ds = 0;
    }

    // seconds, then minutes, then hours
    setTimepart(0, 11, 12, 23, 1, 10, 13, 22, osec);
    setTimepart(2, 9, 14, 21, 3, 9, 15, 20, 23);
    setTimepart(4, 7, 16, 19, 5, 6, 17, 18, 15);

    Serial.println("--------------");
    for (int i =0; i < PIXEL_COUNT; i++ ) {
        //Serial.print(timeBlock[i]);
        //Serial.print(" -- ");
    }
    Serial.println("===");

    change++;
    if (change >= 5) {
    for (int y = 0; y < PIXEL_COUNT; y++) {
        int color[3];
        if (timeBlock[y] == 0) {
            get_color(anti_colors[ds], color);
        } else {
            get_color(colors[ds], color);
        }
        pixels.setPixelColor(y, color[0], color[1], color[2]);
    }
        change = 0;
    }
    //Serial.println(pixels);
    pixels.show();

    delay(500);
}
