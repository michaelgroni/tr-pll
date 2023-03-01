import io
import machine
import micropython as mp
import os
import time

# stoff form others
import mcp4725

# my stuff
import internalInput
import internalOutput
import userInput
from display import Display
from scanner import Scanner
from subtone import Subtone
from vfo import Vfo

_DEBOUNCE_TIME = const(120) # milliseconds

scanner = Scanner()
subtone = Subtone()


#i2c
sda = machine.Pin(0)
scl = machine.Pin(1)
i2c=machine.I2C(0, sda=sda, scl=scl, freq=2000000)


# display
display = Display(i2c)


#dac
dac=mcp4725.MCP4725(i2c, 96)


# interrupt handler encoeder button

def encoderButtonHandler(p): # toggle scanner
    scanner.setOn(not scanner.isOn())

userInput.encoderButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=encoderButtonHandler)


# interrupt handler up
# Pressing up means the same as turnig the rotary encoder one tick clockwise.

lastUp = 0 # timestamp for debouncing

def upHandler(p):
    global lastUp
    now = time.ticks_ms()
    if time.ticks_diff(now, lastUp) >= _DEBOUNCE_TIME:
        lastUp = now
        encoder.up()
        scanner.setUp(True)
        mp.schedule(beepOK(), None)
        
userInput.upButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=upHandler)


# interrupt handler down
# Pressing down means the same as turnig the rotary encoder one tick counterclockwise.

lastDown = 0 # timestamp for debouncing

def downHandler(p):
    global lastDown
    now = time.ticks_ms()
    if time.ticks_diff(now, lastDown) >= _DEBOUNCE_TIME:
        lastDown = now
        encoder.down()
        scanner.setUp(False)
        mp.schedule(beepOK(), None)
        
userInput.downButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=downHandler)


# interrupt handler scan button, used as stepDecrease

lastStepDecrease = 0 # timestamp for debouncing

def stepDecreaseHandler(p):
    global frequencyChanged, lastStepDecrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepDecrease) >= _DEBOUNCE_TIME:
        lastStepDecrease = now
        if not frontInput.memoryActive():
            mp.schedule(beepOK(), None)
            vfo.incStep(-1)
            frequencyChanged = True
        else:
            mp.schedule(beepError(), None)
userInput.stepDecreaseButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=stepDecreaseHandler)


# interrupt handler hold button, used as stepIncrease

lastStepIncrease = 0 # timestamp for debouncing

def stepIncreaseHandler(p):
    global frequencyChanged, lastStepIncrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepIncrease) >= _DEBOUNCE_TIME:
        lastStepIncrease = now
        if not frontInput.memoryActive():
            mp.schedule(beepOK(), None)
            vfo.incStep(1)
            frequencyChanged = True
        else:
            mp.schedule(beepError(), None)
            
userInput.stepIncreaseButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=stepIncreaseHandler)


# interrupt handler ptt button
def pttHandler(p):
    scanner.setOn(False)
    
userInput.pttButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=pttHandler)


# memory defaults

vfoMemory = [Vfo(145275000, 145275000, subtone.getIndex(), False)]
vfoMemory.append(Vfo(145400000, 145400000, subtone.getIndex(), False))
vfoMemory.append(Vfo(145575000, 144975000, 17, True))  # 123 Hz
vfoMemory.append(Vfo(145600000, 145000000, subtone.getIndex(), False))
vfoMemory.append(Vfo(145650000, 145050000, 21, True))  # 141,3 Hz
vfoMemory.append(Vfo(145750000, 145150000, 17, True))  # 123 Hz


# load memories

if "memory.csv" in os.listdir():
    with io.open("memory.csv", mode='r') as csvfile:
        i = 0
        for line in csvfile.readlines():
            values = line.split(";")

            rxFrequency = int(values[0])
            txFrequency = int(values[1])
            subtoneIndex = int(values[2])
            subtoneOn = values[3].strip() == "True"
            
            vfoMemory[i] = Vfo(rxFrequency, txFrequency, subtoneIndex, subtoneOn)
            i += 1
       
       
# vfos
        
vfoA = Vfo(145400000, 145400000, subtone.getIndex(), False)
vfoB = Vfo(144375000, 144375000, subtone.getIndex(), False)

def currentVfo():
    if  not userInput.isPressed(userInput.mrSwitch): # MR not pressed, normal vfo operation
        if userInput.isPressed(userInput.abSwitch):
            return vfoB
        else:
            return vfoA
    else:
        return vfoMemory[userInput.memoryChannel()-1]
    
    
    
# interrupt handler M button (write memory)

def mHandler(p):
    if userInput.memoryActive(): # it makes no sense to write a memory channel when the memory ist active
        mp.schedule(beepEror(), None)
        return
    else:
        for i in range(9):
            utime.sleep_ms(100) # the user has to press the button for some time
            if frontInput.mButton.value(): # button released too early
                mp.schedule(beepError(), None)
                return
        mp.schedule(beepWriteOK(), None)
        vfoMemory[userInput.memoryChannel()-1] = currentVfo()
        with io.open("memory.csv", mode='w') as csvfile:
            for v in vfoMemory:
                csvfile.write(v.toCsvString() + "\n")
                
userInput.mButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=mHandler)


# main loop

internalOutput.setTxForbidden(True)
internalOutput.initPLL()

frequencyChanged = True # makes the main loop update display and pll
memScanOn = userInput.isPressed(userInput.msSwitch)
memoryPressed = userInput.isPressed(userInput.mrSwitch)
oldMemoryChannel = 0

vfo = currentVfo()

while True:
    time.sleep_ms(25)
    
    # vfo
    if not userInput.memoryActive(): # if no memory switch is pressed, the transceiver works in vfo mode
        if ((vfo is vfoA  and userInput.isPressed(userInput.abSwitch)) or vfo is vfoB and not userInput.isPressed(userInput.abSwitch)): # vfo changed
            beepOK()
            time.sleep_ms(_DEBOUNCE_TIME)
            vfo = currentVfo()
            frequencyChanged=True
            subtone.setIndex(currentVfo().getSubtoneIndex())
            
    # memory
    elif not userInput.isPressed(userInput.msSwitch):
        if memScanOn: # MS button released
            memScanOn = False
            beepOK()
            utime.sleep_ms(_DEBOUNCE_TIME)
    else:   # MS pressed - memory scan
        if not memScanOn:
            memScanOn = True
            beepOK()
            utime.sleep_ms(_DEBOUNCE_TIME)
        txForbidden.value(1)
        time.sleep_ms(63)   # squelch needs some time to close
        if squelchOpen.value():
            utime.sleep_ms(3000)         
        memoryScanChannel += 1
        if memoryScanChannel == 7:
            memoryScanChannel = 1
        vfo = vfoMemory[memoryScanChannel-1]
        subtone.setIndex(vfo.getSubtoneIndex())
        frequencyChanged = True
      
    if userInput.isPressed(userInput.mrSwitch) and not memScanOn: # memory scan wins over memory read
        vfo = currentVfo()
        
        if not memoryPressed: # switch position changed
            memoryPressed = True
            frequencyChanged = True
            beepOK()
        
        currentChannel = userInput.memoryChannel()
        if oldMemoryChannel != currentChannel: # memory channel changed
            oldMemoryChannel = currentChannel
            subtone.setIndex(vfo.getSubtoneIndex())
            frequencyChanged = True
        
        if (vfo.getRxFrequency() != vfo.getTxFrequency()) and not userInput.isPressed(userInput.pttButton):
            internalOutput.setTtxForbidden(True)  # no TX allowed as long as PLL ist not configured
    else: # neither MR nor MS pressed
         if memoryPressed:
            memoryPressed = False
            frequencyChanged = True
            beepOK()      