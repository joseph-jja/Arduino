import board
import digitalio
import time
from analogio import AnalogIn

led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

distance = AnalogIn(board.A1)

while True:
    voltage = ( distance.value * 3.3 ) / 65536
    print (voltage)
    led.value = True
    time.sleep(0.5)
    led.value = False
    time.sleep(0.5)
