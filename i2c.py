import machine

import mcp4725

from display import Display


sda = machine.Pin(0, pull=machine.Pin.PULL_UP)
scl = machine.Pin(1, pull=machine.Pin.PULL_UP)
i2c=machine.I2C(0, sda=sda, scl=scl, freq=400000) # PCF8574 fmax = 100 kHz


# display
display = Display(i2c)


# dac (tunes drive unit)
dac=mcp4725.MCP4725(i2c, 96)


def readData(): # must be called in the main loop
    global byte1, byte2
    byte1= i2c.readfrom(32, 1)[0]  # IC3
    byte2 = i2c.readfrom(33, 1)[0] # IC4


# memory switch
def memoryChannel():    
    if byte2 & 4 == 0:
        return 6
    if byte2 & 8 == 0:
        return 5
    if byte1 & 1 == 0:
        return 4
    if byte1 & 2 == 0:
        return 3
    if byte1 & 4 == 0:
        return 2
    
    return 1


def mode():    
    if byte1 & 8 == 0:  # LSB
        return 4
    if byte1 & 16 == 0: # FM1
        return 1
    if byte1 & 32 == 0: # CTCSS (FM2)
        return 2
    # else USB or CW
    return 3


# +/- switch
def duplexOffset():
    if mode()<3:  # FM    
        if byte1 & 64 == 0:
            return -600000
        if byte1 & 128 == 0:
            return 600000
        return 0
    else:
        return 0