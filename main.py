import io
import machine
import micropython as mp
import os
import time


# my stuff
import beep
import internalInput
import internalOutput
import userInput
from ctcss import ctcss, ctcssMachine
from scanner import Scanner
from subtone import Subtone
from vfo import Vfo
from rotaryEncoder import RotaryEncoder

_DEBOUNCE_TIME = const(120) # milliseconds

scanner = Scanner()
subtone = Subtone()

encoder = RotaryEncoder(userInput.rotaryClockPin, userInput.rotaryDataPin)


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
        mp.schedule(beep.beepOK(), None)
        
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
        mp.schedule(beep.beepOK(), None)
        
userInput.downButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=downHandler)


# interrupt handler scan button, used as stepDecrease

lastStepDecrease = 0 # timestamp for debouncing

def stepDecreaseHandler(p):
    global frequencyChanged, lastStepDecrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepDecrease) >= _DEBOUNCE_TIME:
        lastStepDecrease = now
        if not userInput.memoryActive():
            mp.schedule(beep.beepOK(), None)
            vfo.incStep(-1)
            frequencyChanged = True
        else:
            mp.schedule(beep.beepError(), None)
userInput.stepDecreaseButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=stepDecreaseHandler)


# interrupt handler hold button, used as stepIncrease

lastStepIncrease = 0 # timestamp for debouncing

def stepIncreaseHandler(p):
    global frequencyChanged, lastStepIncrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepIncrease) >= _DEBOUNCE_TIME:
        lastStepIncrease = now
        if not userInput.memoryActive():
            mp.schedule(beep.beepOK(), None)
            vfo.incStep(1)
            frequencyChanged = True
        else:
            mp.schedule(beep.beepError(), None)
            
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

def saveMemory():
    with io.open("memory.csv", mode='w') as csvfile:
        for i in vfoMemory:
            csvfile.write(i.toCsvString() + "\n")    


def mHandler(p):
    if userInput.memoryActive(): # it makes no sense to write a memory channel when the memory ist active
        mp.schedule(beep.beepError(), None)
        return
    else:
        for i in range(9):
            time.sleep_ms(100) # the user has to press the button for some time
            if not userInput.isPressed(userInput.mButton): # button released too early
                mp.schedule(beep.beepError(), None)
                return
        mp.schedule(beep.beepWriteOK(), None)
        while userInput.isPressed(userInput.mButton): # wait until m-Button is released
            time.sleep_ms(_DEBOUNCE_TIME)             # TODO improve hardware
        vfoMemory[userInput.memoryChannel()-1] = currentVfo()
        mp.schedule(saveMemory(), None)

                
userInput.mButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=mHandler)


# main loop

internalOutput.setTxForbidden(True)
internalOutput.initPLL()

frequencyChanged = True # makes the main loop update display and pll
memScanOn = userInput.isPressed(userInput.msSwitch)
memoryScanChannel = 1  # 1..6
memoryPressed = userInput.isPressed(userInput.mrSwitch)

oldMemoryChannel = 0
oldMode = 0
oldPTTpressed = False
oldDuplexOffset = 0
oldReverseMode = 1

vfo = currentVfo()

while True:
    time.sleep_ms(25)
    
    # vfo
    if not userInput.memoryActive(): # if no memory switch is pressed, the transceiver works in vfo mode
        if ((vfo is vfoA  and userInput.isPressed(userInput.abSwitch)) or vfo is vfoB and not userInput.isPressed(userInput.abSwitch)): # vfo changed
            beep.beepOK()
            time.sleep_ms(_DEBOUNCE_TIME)
            vfo = currentVfo()
            frequencyChanged=True
            subtone.setIndex(currentVfo().getSubtoneIndex())
            
    # memory
    elif not userInput.isPressed(userInput.msSwitch):
        if memScanOn: # MS button released
            memScanOn = False
            beep.beepOK()
            time.sleep_ms(_DEBOUNCE_TIME)
    else:   # MS pressed - memory scan
        if not memScanOn:
            memScanOn = True
            beep.beepOK()
            time.sleep_ms(_DEBOUNCE_TIME)
        internalOutput.setTxForbidden(True)
        time.sleep_ms(63)   # squelch needs some time to close
        if internalInput.isSquelchOpen():
            time.sleep_ms(3000)         
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
            beep.beepOK()
        
        currentChannel = userInput.memoryChannel()
        if oldMemoryChannel != currentChannel: # memory channel changed
            oldMemoryChannel = currentChannel
            subtone.setIndex(vfo.getSubtoneIndex())
            frequencyChanged = True
        
        if (vfo.getRxFrequency() != vfo.getTxFrequency()) and not userInput.isPressed(userInput.pttButton):
            internalOutput.setTxForbidden(True)  # no TX allowed as long as PLL ist not configured
    else: # neither MR nor MS pressed
         if memoryPressed:
            memoryPressed = False
            frequencyChanged = True
            vfo = currentVfo()
            beep.beepOK()
        
        
    # mode changed?
    currentMode = userInput.mode()
    if oldMode != currentMode:
        beep.beepOK()
        frequencyChanged = True
        oldMode = currentMode
    
    
    # ptt changed?        
    currentPTTpressed = userInput.isPressed(userInput.pttButton)
    if oldPTTpressed != currentPTTpressed:
        frequencyChanged = True
        oldPTTpressed = currentPTTpressed
    
    
    # ctcss subtone
    if not userInput.memoryActive(): # memory channels use their own subtone
        vfo.setSubtoneOn(userInput.mode()==1)  # subtone on if mode is FM1
            
    if subtone.changed():
        subtone.changeProcessed()
        ctcssMachine.init(ctcss, freq=int(subtone.value()*180), set_base=machine.Pin(10))
    
    if vfo.isSubtoneOn() and userInput.isPressed(userInput.pttButton):
        ctcssMachine.active(1)
    else:
        ctcssMachine.active(0)
        
        
    # scanner
    if scanner.isOn() and not userInput.memoryActive():
        internalOutput.setTxForbidden(True)
        if internalInput.isSquelchOpen():
            time.sleep_ms(4000)
        if scanner.isUp():
            vfo.setRxFrequency(vfo.getRxFrequency() + vfo.step())
        else:
            vfo.setRxFrequency(vfo.getRxFrequency() - vfo.step())
        frequencyChanged = True
        
        
    # duplex offset
    if not userInput.memoryActive(): # memory channels use their own offset
        currentDuplexOffset = userInput.duplexOffset()
        if oldDuplexOffset != currentDuplexOffset:
            frequencyChanged = True
            oldDuplexOffset = currentDuplexOffset
        if (currentDuplexOffset != 0) and not userInput.isPressed(userInput.pttButton):
            internalOutput.setTxForbidden(True)  # no TX allowed as long as PLL ist not configured
            
    
    # rotary encoder
    rotaryValue = encoder.getValue()
    if rotaryValue != 0:
        encoder.reset()
        if not userInput.memoryActive():
            if userInput.mode()==1:    # FM1 = FM with subtone
                subtone.add(rotaryValue)
                vfo.setSubtoneIndex(subtone.getIndex())
            elif not userInput.isPressed(userInput.pttButton):  # encoder not in ctcss mode and ptt not pressed
                vfo.setRxFrequency(vfo.getRxFrequency() + rotaryValue*vfo.step())
                frequencyChanged = True
        else: # memory active
            beep.beepError()


    # reverse button
    currentReverseMode = userInput.isPressed(userInput.reverseButton)
    if oldReverseMode != currentReverseMode:
        frequencyChanged = True
        beep.beepOK()
        time.sleep_ms(_DEBOUNCE_TIME)
        oldReverseMode = currentReverseMode
        
    
    # frequency changed?
    if frequencyChanged:
        frequencyChanged = False
        
        if not userInput.memoryActive():
            if vfo.getRxFrequency()<143000000:
                vfo.setRxFrequency(150000000)
            elif vfo.getRxFrequency()>150000000:
                vfo.setRxFrequency(143000000)
        
            if vfo.step()==12500:
                vfo.setRxFrequency(vfo.getRxFrequency() - vfo.getRxFrequency() % 12500)       
            vfo.setTxFrequency(vfo.getRxFrequency() + currentDuplexOffset)
        
        if not userInput.isPressed(userInput.pttButton) and not userInput.isPressed(userInput.reverseButton):
                currentFrequency = vfo.getRxFrequency()
        else: # ptt pressed or reverse pressed
            currentFrequency = vfo.getTxFrequency()
            
            
        # PLL
        internalOutput.setTxForbidden(True)
        
        if currentMode == 3:   # USB or CW
            pllFrequency = currentFrequency + 1500
        elif currentMode == 4: # LSB
            pllFrequency = currentFrequency -1500
        else:
            pllFrequency = currentFrequency
            
        pllFrequency = pllFrequency - 10695000
        
        nPll = pllFrequency * 80 / 1000000
        intPll = int(nPll)
        fracPll = int(1250 * (nPll-intPll))
        r0Pll = (intPll << 15) + (fracPll << 3)
        # R5
        internalOutput.writePLL(internalOutput.spi, bytearray(b'\x00\x58\x00\x05'))
        # R4
        if not userInput.isPressed(userInput.pttButton): 
            internalOutput.writePLL(internalOutput.spi, bytearray(b'\x00\xD0\x14\x3C')) # more power from PLL in RX mode
        else:
            internalOutput.writePLL(internalOutput.spi, bytearray(b'\x00\xD0\x14\x34'))
        # R3
        internalOutput.writePLL(internalOutput.spi, bytearray(b'\x00\x80\x04\xB3'))
        # R2
        internalOutput.writePLL(internalOutput.spi, bytearray(b'\x00\x06\x4E\x42'))
        # R1
        internalOutput.writePLL(internalOutput.spi, bytearray(b'\x08\x00\xA7\x11'))
        # R0
        internalOutput.writePLL(internalOutput.spi, r0Pll.to_bytes(4, 'big'))
        
        # tune drive unit and allow tx
        if vfo.getTxFrequency() >= 144000000 and vfo.getTxFrequency() < 146000000 \
        and userInput.isPressed(userInput.pttButton) \
        and not scanner.isOn() \
        and not userInput.isPressed(userInput.msSwitch) \
        and not userInput.isPressed(userInput.reverseButton):
            # dac Vref = 3.3 V
            dacVoltage = (currentFrequency / 2000000.0) - 70.9
            dacValue = int(dacVoltage / 3.3 * 4096)
            internalOutput.dac.write(dacValue)
            
            internalOutput.setTxForbidden(False)
            
        # display line 1
        internalOutput.display.setFrequency(currentFrequency)
        
    
    # display offset
    if userInput.memoryActive():
        internalOutput.display.setOffset(vfo.getTxFrequency() - vfo.getRxFrequency())
    else: # transceiver in vfo mode
        internalOutput.display.setOffset(currentDuplexOffset)
    
    
    # display line 2
    if userInput.isPressed(userInput.msSwitch):
        internalOutput.display.setLine2("Memory Scan " + str(memoryScanChannel))
    elif userInput.isPressed(userInput.mrSwitch):
        internalOutput.display.setLine2("Memory " + str(userInput.memoryChannel()))
    else:  # no memory operation
        internalOutput.display.setLine2("Step " + vfo.stepToString())


    # display line 3
    if scanner.isOn():
        internalOutput.display.setLine3("Scan")
    elif vfo.isSubtoneOn() and not userInput.isPressed(userInput.msSwitch):
        internalOutput.display.setLine3("CTCSS " + "{0:1}".format(subtone.value()))
    else:
        internalOutput.display.setLine3("");