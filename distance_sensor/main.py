
from time import sleep
from analogio import AnalogIn
import board
import adafruit_dotstar
import neopixel

# distance sensor
distance_sensor = AnalogIn(board.A2)

# onboard LED
led = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)

# neopixels
pixel_pin = board.A4
num_pixels = 19
pixels = neopixel.NeoPixel(pixel_pin, num_pixels, brightness=0.3, auto_write=False)

# millivolts per inch
mvPerInch = 10

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

ORANGE = (255, 0, 150)

colors = {
    0: RED,
    1: LIGHT_RED,
    2: YELLOW,
    3: GREEN,
    4: CYAN,
    5: LIGHT_BLUE,
    6: BLUE,
    7: PINKISH,
    8: PURPLE,
    9: WHITE,
    10: ORANGE
}

colorCount = 10

def get_voltage(pin):
    ref_vlt = pin.reference_voltage
    #print(pin.value)
    #print(pin.value/512 * 3.3)
    #return (pin.value / ref_vlt)
    return (pin.value/512 * 3.3)

def change_dot_star(x):
    led[0] = colors.get(x)
    sleep(1)

x = 0
p = 0
ds = 0
while True:
    sv = get_voltage(distance_sensor)
    print(sv)

    # this is the little neopixel on the board
    change_dot_star(ds)
    ds = ds + 1
    if (ds > colorCount ):
        ds = 0

    for y in range(num_pixels):
        pixels[y] = colors.get(x)
        if ( y == 4 or y == 14 ):
            pixels[y] = colors.get(ds)
        x = x + 1
        if (x > colorCount ):
            x = 0
    pixels.show()

    p = p + 1
    if ( p > num_pixels ):
        p = 0
    # Increase or decrease to change the speed of the solid color change.
    sleep(0.25)