from math import floor
from time import sleep
import board
import adafruit_dotstar
import neopixel
import busio

# onboard LED
led = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)

# neopixels
pixel_pin = board.A4
num_pixels = 24
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.7, auto_write=False)

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

anti_colors = {
    0: LIGHT_BLUE,
    1: BLUE,
    2: GREEN,
    3: YELLOW,
    4: PURPLE,
    5: LIGHT_RED,
    6: RED,
    7: CYAN,
    8: BLACK,
    9: WHITE
}

colorCount = 9

decimal2binary = {
    0: [0, 0, 0, 0],
    1: [0, 0, 0, 1],
    2: [0, 0, 1, 0],
    3: [0, 0, 1, 1],
    4: [0, 1, 0, 0],
    5: [0, 1, 0, 1],
    6: [0, 1, 1, 0],
    7: [0, 1, 1, 1],
    8: [1, 0, 0, 0],
    9: [1, 0, 0, 1]
}

timeBlock = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# 18 19 20 21 22 23
# 17 16 15 14 13 12
#  6  7  8  9 10 11
#  5  4  3  2  1  0
def setTimepart(a, b, c, d, e, f, g, h, timeIn):

    tens = floor(timeIn/10)
    remainder = timeIn - (tens*10)

    tensx = decimal2binary[tens]
    secx = decimal2binary[remainder]

    timeBlock[a] = secx[0]
    timeBlock[b] = secx[1]
    timeBlock[c] = secx[2]
    timeBlock[d] = secx[3]

    timeBlock[e] = tensx[0]
    timeBlock[f] = tensx[1]
    timeBlock[g] = tensx[2]
    timeBlock[h] = tensx[3]

def change_dot_star(x):
    led[0] = colors.get(x)
    sleep(1)

x = 0
p = 0
ds = 0
sec = 0
while True:

    # this is the little neopixel on the board
    change_dot_star(ds)
    ds = ds + 1
    if (ds > colorCount ):
        ds = 0

    # seconds, then minutes, then hours
    setTimepart(0, 11, 12, 23, 1, 10, 13, 22, sec)
    setTimepart(2, 9, 14, 21, 3, 9, 15, 20, 23)
    setTimepart(4, 7, 16, 19, 5, 6, 17, 18, 45)

    print ("--------------")
    print (sec, timeBlock[0], timeBlock[11], timeBlock[12], timeBlock[23])

    for y in range(num_pixels):
        if (timeBlock[y] == 0):
            pixels[y] = anti_colors.get(ds)
        else:
            pixels[y] = colors.get(ds)

    pixels.show()

    sec = sec + 1
    if (sec >= 60):
        sec = 0

    # Increase or decrease to change the speed of the solid color change.
    sleep(5)

