#pragma once

#include <stdlib.h>
#include <string.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// For SSD1306: Ensure the correct address (0x3C or 0x3D) is passed
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Or an unused GPIO pin, like D0 (GPIO16)
#define SCREEN_ADDRESS 0x3C 
