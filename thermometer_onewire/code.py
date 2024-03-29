import board
import time
from time import sleep
from adafruit_ht16k33.segments import Seg14x4
from busio import I2C
from adafruit_onewire.bus import OneWireBus
import adafruit_ds18x20
from math import floor
from digitalio import DigitalInOut, Direction, Pull

resistor_pin = DigitalInOut(board.D6)
resistor_pin.direction = Direction.INPUT

high_pin = DigitalInOut(board.D4)
high_pin.direction = Direction.OUTPUT
high_pin.value = True

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

i = 0
while True:
    
    # so I don't know if this is the correct formula
    # it seems to be better than what I read :)
    celsius = ds18b20.temperature
    fahrenheit = (celsius * 1.80) + 32.00;

    celsius = floor(celsius * 10) / 10
    fahrenheit = floor(fahrenheit * 10) / 10

    celsiusStr = str(celsius) + "C"
    fahrenheitStr = str(fahrenheit) + "F"
    if celsius > 100:
        celsius = floor(celcius)
        celsiusStr = str(celsius) + "C"

    if fahrenheit > 100:
        fahrenheit = floor(fahrenheit)
        fahrenheitStr = str(fahrenheit) + "F"

    print("Got temperature C", celsius)
    print("Got temperature F", fahrenheit)
    display.fill(0)

    if resistor_pin.value == True:
        print ("Got input button", resistor_pin.value)
        i = 1
        #resistor_pin.value = False
    
    if i > 0:
        display.print(celsiusStr)
        sleep(1)
        display.print(fahrenheitStr)
        sleep(1)
        i = i + 1
        
    if i > 20:
        i = 0
        
    print("Value of i is", i)
    print("Value of i is", i)
