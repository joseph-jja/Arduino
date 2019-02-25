import board
import digitalio
import time
 
led = digitalio.DigitalInOut(board.D13)
led.direction = digitalio.Direction.OUTPUT

// distance = digitalio.DigitalInOut(1)
// distance.direction = digitalio.Direction.INPUT

while True:
    val = distance.value
    led.value = True
    time.sleep(0.5)
    led.value = False
    time.sleep(0.5)
