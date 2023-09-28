import board
import time
from time import sleep
from adafruit_ht16k33.segments import Seg14x4
from busio import I2C
from adafruit_onewire.bus import OneWireBus
import adafruit_ds18x20
from math import floor

ow_bus = OneWireBus(board.D5)
print("starting...")

# Seven Segment FeatherWing setup
i2c = I2C(board.D1, board.D0)

if i2c.try_lock():

    for x in i2c.scan():
        print("testing", x, hex(x))
    i2c.unlock()

display = Seg14x4(i2c)
#display.brightness = 15.0
display.fill(1)  # Test
sleep(1)
display.fill(0)  # Clear the display

devices = ow_bus.scan()
print(devices)
for device in devices:
    print("ROM = {} \tFamily = 0x{:02x}".format([hex(i) for i in device.rom], device.family_code))

ds18b20 = adafruit_ds18x20.DS18X20(ow_bus, devices[0])

while True:
    # so I don't know if this is the correct formula
    # it seems to be better than what I read :)
    value = ds18b20.temperature
    print("Got temperature", value)
    display.fill(0)
    
    display.print(floor(value))

    sleep(0.25)
