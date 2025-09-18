from math import floor
from time import sleep
import board
import adafruit_dotstar
import neopixel

# onboard LED
led = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)

# neopixels
pixel_pin = board.A4
num_pixels = 7
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.5, auto_write=False)

RED = (255, 0, 0)
LIGHT_RED = (150, 0, 0)
YELLOW = (255, 255, 0)
GREEN = (0, 255, 0)
CYAN = (0, 255, 255)
BLUE = (0, 0, 255)
LIGHT_BLUE = (0, 255, 150)
PINKISH = (255, 0, 150)
PURPLE = (255, 0, 255)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

colors = {
    0: RED,
    1: LIGHT_RED,
    2: YELLOW,
    3: GREEN,
    4: CYAN,
    5: LIGHT_BLUE,
    6: BLUE,
    7: PURPLE,
    8: WHITE,
    9: BLACK
}

colorCount = 8

ds = 0
while True:

    # this is the little neopixel on the board
    led[0] = colors.get(ds)

    for y in range(num_pixels):
        pixels[y] = colors.get(ds)

    ds = ds + 1
    if (ds > colorCount ):
        ds = 0

    pixels.show()

    # Increase or decrease to change the speed of the solid color change.
    sleep(2)

