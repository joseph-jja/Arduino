from array import *

decimal2hex = {
    0: [0, 0, 0, 0],
    1: [0, 0, 0, 1], 
    2: [0, 0, 1, 0], 
    3: [0, 0, 1, 1], 
    4: [0, 1, 0, 0], 
    5: [0, 1, 0, 1], 
    6: [0, 1, 1, 0], 
    7: [0, 1, 1, 1], 
    8: [1, 0, 0, 0], 
    9: [1, 0, 0, 1], 
    10: [1, 0, 1, 0], 
    11: [1, 0, 1, 1], 
    12: [1, 1, 0, 0], 
    13: [1, 1, 0, 1], 
    14: [1, 1, 1, 0], 
    15: [1, 1, 1, 1]
}

timeBlock = array('i', [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

# 18 19 20 21 22 23
# 17 16 15 14 13 12
#  6  7  8  9 10 11
#  5  4  3  2  1  0
def setSeconds(timeIn):

    tens = round(timeIn/10)
    remainder = timeIn - (tens*10)

    tensx = decimal2hex[tens]
    secx = decimal2hex[remainder]

    timeBlock[0] = secx[0];
    timeBlock[11] = secx[1];
    timeBlock[12] = secx[2];
    timeBlock[23] = secx[3];

    timeBlock[1] = tensx[0];
    timeBlock[10] = tensx[1];
    timeBlock[13] = tensx[2];
    timeBlock[22] = tensx[3];

def setMinutes(timeIn):

    tens = round(timeIn/10)
    remainder = timeIn - (tens*10)

    tensx = decimal2hex[tens]
    secx = decimal2hex[remainder]

    timeBlock[2] = secx[0]
    timeBlock[9] = secx[1]
    timeBlock[14] = secx[2]
    timeBlock[21] = secx[3]

    timeBlock[3] = tensx[0]
    timeBlock[8] = tensx[1]
    timeBlock[15] = tensx[2]
    timeBlock[20] = tensx[3]

def setHours(timeIn):

    tens = round(timeIn/10)
    remainder = timeIn - (tens*10)

    tensx = decimal2hex[tens]
    secx = decimal2hex[remainder]

    timeBlock[4] = secx[0]
    timeBlock[7] = secx[1]
    timeBlock[16] = secx[2]
    timeBlock[19] = secx[3]

    timeBlock[5] = tensx[0]
    timeBlock[6] = tensx[1]
    timeBlock[17] = tensx[2]
    timeBlock[18] = tensx[3]

setSeconds(12)
setHours(1)
setMinutes(21)

