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
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=1, auto_write=False)

RED = (255, 0, 0)
ORANGE = (255, 50, 0)
YELLOW = (255, 255, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
PURPLE = (255, 0, 255)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

colors = {
    0: RED,
    1: ORANGE,
    2: YELLOW,
    3: GREEN,
    4: BLUE,
    5: PURPLE,
    6: WHITE,
    7: BLACK
}

colorCount = 7

ds = 0
pxl = 0
while True:

    # this is the little neopixel on the board
    led[0] = colors.get(ds)
    print(pixels)

    for y in range(num_pixels):
        pixels[pxl] = colors.get(ds)

    pxl = pxl + 1
    if (pxl >= num_pixels ):
        pxl = 0

    #ds = ds + 1
    if (ds > colorCount ):
        ds = 0

    pixels.show()

    # Increase or decrease to change the speed of the solid color change.
    sleep(1)