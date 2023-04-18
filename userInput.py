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
reverseButton = machine.Pin(18, machine.Pin.IN, machine.Pin.PULL_UP)


def isPressed(buttonOrSwitch):
    return buttonOrSwitch.value() == 0
    
    
def memoryActive():
    return isPressed(mrSwitch) or isPressed(msSwitch)