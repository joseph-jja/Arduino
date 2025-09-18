import board
import digitalio
import time
from time import sleep
from analogio import AnalogIn
from adafruit_ht16k33.segments import Seg14x4
from busio import I2C
from math import floor

distance = AnalogIn(board.A0)
#print("starting...")

# Seven Segment FeatherWing setup
i2c = I2C(board.D1, board.D0)

if i2c.try_lock():
    
    for x in i2c.scan():
        print("testing")
        print(x, hex(x))
    i2c.unlock()
display = Seg14x4(i2c)
#display.brightness = 15
display.fill(1)  # Test
sleep(1)
display.fill(0)  # Clear the display

feet_convert = 147 * 12

array_of_values = []
i = 0

while True:
    # so I don't know if this is the correct formula
    # it seems to be better than what I read :)
    value = distance.value
    voltage = floor(((value * distance.reference_voltage) / 65536) * 100) / 10
    feets = floor((value / feet_convert) * 10) / 10
    average = (feets + voltage) / 2
    array_of_values.append(average)
    
    if i > 6:
        i = 0
        array_of_values.sort()
        print("Approximate distance: ", voltage, feets, average, array_of_values[3])
        display.fill(0)
        display_value = str(array_of_values[3])
        display.print(display_value)
        array_of_values.clear()
    
    i = i + 1
    sleep(0.25)

