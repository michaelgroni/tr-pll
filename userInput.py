import machine

# rotary encoder
rotaryClockPin = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)
rotaryDataPin = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_UP)
encoderButton = machine.Pin(22, machine.Pin.IN, machine.Pin.PULL_UP)

# mic buttons
upButton = machine.Pin(13, machine.Pin.IN, machine.Pin.PULL_UP)
downButton = machine.Pin(12, machine.Pin.IN, machine.Pin.PULL_UP)
pttButton = machine.Pin(20, machine.Pin.IN)

# front buttons and switches
stepDecreaseButton = machine.Pin(17, machine.Pin.IN, machine.Pin.PULL_UP) # formerly scan button
stepIncreaseButton = machine.Pin(16, machine.Pin.IN, machine.Pin.PULL_UP) # formerly hold button
mrSwitch = machine.Pin(8, machine.Pin.IN, machine.Pin.PULL_UP)            # memory read
msSwitch = machine.Pin(9, machine.Pin.IN, machine.Pin.PULL_UP)            # memory scan
mButton = machine.Pin(19, machine.Pin.IN, machine.Pin.PULL_UP)            # memory write
abSwitch = machine.Pin(11, machine.Pin.IN, machine.Pin.PULL_UP)           # 1 = vfoA    0 = pressed = vfoB
modeSwitch = machine.ADC(27)
offsetSwitch = machine.ADC(26)
reverseButton = machine.Pin(18, machine.Pin.IN, machine.Pin.PULL_UP)

# analogue front input
memoryInput = machine.ADC(28)


def isPressed(buttonOrSwitch):
    return buttonOrSwitch.value() == 0
    

def memoryChannel():
    memoryAdc = memoryInput.read_u16()
    
    if memoryAdc < 10000:
        return 1
    elif memoryAdc < 20000:
        return 2
    elif memoryAdc < 27000:
        return 3
    elif memoryAdc < 33000:
        return 4
    elif memoryAdc < 40000:
        return 5
    else:
        return 6
    
    
def memoryActive():
    return isPressed(mrSwitch) or isPressed(msSwitch)

def mode():
    modeAdc = modeSwitch.read_u16()
    
    if modeAdc > 40000:   # FM1
        return 1
    elif modeAdc > 20000: # FM2
        return 2
    elif modeAdc > 13000: # USB or CW
        return 3
    else:                 # LSB
        return 4
    
    
def duplexOffset():
    duplexAdc = offsetSwitch.read_u16()
    if mode()<3:  # FM
        if duplexAdc > 20000:
            return -600000
        elif duplexAdc < 10000:
            return 600000
        else:
            return 0
    else:
        return 0