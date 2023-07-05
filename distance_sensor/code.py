import board
import digitalio
import time
from time import sleep
from analogio import AnalogIn
from adafruit_ht16k33.segments import Seg7x4

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

distance = AnalogIn(board.A1)

# Seven Segment FeatherWing setup
i2c = board.I2C()  # uses board.SCL and board.SDA
display = Seg7x4(i2c, address=0x70)
display.brightness = 15
display.fill(0)  # Clear the display
#display_A.print(1234) will display 1234
#display_A.set_digit_raw(3, 0b00000001) will light up the top segment of the fourth digit (far right) only.
#display[0] = '6'

while True:
    voltage = ( distance.value * 3.3 ) / 65536
    print (voltage)
    led.value = True
    sleep(0.5)
    led.value = False
    sleep(0.5)
