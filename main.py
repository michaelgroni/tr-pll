import io
import machine
import micropython as mp
import os
import time


# my stuff
import beep
import internalInput
import internalOutput
import i2c
import userInput
from ctcss import ctcss, ctcssMachine
from scanner import Scanner
from subtone import Subtone
from vfo import Vfo
from rotaryEncoder import RotaryEncoder



_DEBOUNCE_TIME = const(200) # milliseconds

i2c.readData() # init

scanner = Scanner()
subtone = Subtone()

encoder = RotaryEncoder(userInput.rotaryClockPin, userInput.rotaryDataPin)


# interrupt handler encoeder button

lastEncPress = 0 # timestamp for debouncing

def encoderButtonHandler(p): # toggle scanner
    global lastEncPress
    now = time.ticks_ms()
    if time.ticks_diff(now, lastEncPress) >= _DEBOUNCE_TIME and userInput.isPressed(userInput.encoderButton):
        lastEncPress = now
        scanner.setOn(not scanner.isOn())
        
userInput.encoderButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=encoderButtonHandler)


# interrupt handler up
# Pressing up means the same as turnig the rotary encoder one tick clockwise.

lastUp = 0 # timestamp for debouncing

def upHandler(p):
    global lastUp
    now = time.ticks_ms()
    if time.ticks_diff(now, lastUp) >= _DEBOUNCE_TIME and userInput.upButton.value()==0:
        lastUp = now
        mp.schedule(encoder.up(), None)
        mp.schedule(scanner.setUp(True), None)
        mp.schedule(beep.beepOK(), None)
        
userInput.upButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=upHandler)


# interrupt handler down
# Pressing down means the same as turnig the rotary encoder one tick counterclockwise.

lastDown = 0 # timestamp for debouncing

def downHandler(p):
    global lastDown
    now = time.ticks_ms()
    if time.ticks_diff(now, lastDown) >= _DEBOUNCE_TIME and userInput.downButton.value()==0:
        lastDown = now
        mp.schedule(encoder.down(), None)
        mp.schedule(scanner.setUp(False), None)
        mp.schedule(beep.beepOK(), None)
        
userInput.downButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=downHandler)


# interrupt handler scan button, used as stepDecrease
lastStepDecrease = 0 # timestamp for debouncing

def stepDecreaseHandler(p):
    global lastStepDecrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepDecrease) >= _DEBOUNCE_TIME and userInput.stepDecreaseButton.value()==0:
        lastStepDecrease = now
        if not userInput.memoryActive():
            mp.schedule(beep.beepOK(), None)
            vfo.incStep(-1)
            stepChanged = True
        else:
            mp.schedule(beep.beepError(), None)
userInput.stepDecreaseButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=stepDecreaseHandler)


# interrupt handler hold button, used as stepIncrease

lastStepIncrease = 0 # timestamp for debouncing

def stepIncreaseHandler(p):
    global lastStepIncrease
    now = time.ticks_ms()
    if time.ticks_diff(now, lastStepIncrease) >= _DEBOUNCE_TIME and userInput.stepIncreaseButton.value()==0:
        lastStepIncrease = now
        if not userInput.memoryActive():
            mp.schedule(beep.beepOK(), None)
            vfo.incStep(1)
            stepChanged = True
        else:
            mp.schedule(beep.beepError(), None)
userInput.stepIncreaseButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=stepIncreaseHandler)


# interrupt handler ptt button
def pttHandler(p):
    if userInput.pttButton.value()==0:
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
        return vfoMemory[i2c.memoryChannel()-1]
    
    
    
# interrupt handler M button (write memory)

def saveMemory():
    with io.open("memory.csv", mode='w') as csvfile:
        for i in vfoMemory:
            csvfile.write(i.toCsvString() + "\n")    


lastM = 0 # timestamp for debouncing

def mHandler(p):
    global lastM, vfoMemory
    now = time.ticks_ms()
    if time.ticks_diff(now, lastM) >= _DEBOUNCE_TIME and userInput.isPressed(userInput.mButton):
        lastM = now
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
                time.sleep_ms(_DEBOUNCE_TIME)             
            vfoMemory[i2c.memoryChannel()-1] = currentVfo()
            mp.schedule(saveMemory(), None)

                
userInput.mButton.irq(trigger=machine.Pin.IRQ_FALLING, handler=mHandler)


# main loop

internalOutput.setTxForbidden(True)
internalOutput.initPLL()

memScanOn = userInput.isPressed(userInput.msSwitch)
memoryScanChannel = 1  # 1..6
memoryPressed = userInput.isPressed(userInput.mrSwitch)

oldMemoryChannel = 0
oldMode = 0
oldPTTpressed = False
oldDuplexOffset = 0
oldReverseMode = 1

currentMode = -1

vfo = currentVfo()

while True:
    time.sleep_ms(1)
    
    i2c.readData()
    
    # vfo
    if not userInput.memoryActive(): # if no memory switch is pressed, the transceiver works in vfo mode
        if ((vfo is vfoA  and userInput.isPressed(userInput.abSwitch)) or vfo is vfoB and not userInput.isPressed(userInput.abSwitch)): # vfo changed
            beep.beepOK()
            time.sleep_ms(_DEBOUNCE_TIME)
            vfo = currentVfo()
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
      
    if userInput.isPressed(userInput.mrSwitch) and not memScanOn: # memory scan wins over memory read
        vfo = currentVfo()
        
        if not memoryPressed: # switch position changed
            memoryPressed = True
            beep.beepOK()
        
        currentChannel = i2c.memoryChannel()
        if oldMemoryChannel != currentChannel: # memory channel changed
            oldMemoryChannel = currentChannel
            subtone.setIndex(vfo.getSubtoneIndex())
        
        if (vfo.getRxFrequency() != vfo.getTxFrequency()) and not userInput.isPressed(userInput.pttButton):
            internalOutput.setTxForbidden(True)  # no TX allowed as long as PLL ist not configured
    else: # neither MR nor MS pressed
         if memoryPressed:
            memoryPressed = False
            vfo = currentVfo()
            beep.beepOK()
        
        
    # mode changed?
    currentMode = i2c.mode()
    if currentMode == 3: # mode is USB/CW or switch is being moved at the moment
        time.sleep_ms(_DEBOUNCE_TIME)
        i2c.readData()
        currentMode = i2c.mode()
    
    
    # ptt changed?        
    currentPTTpressed = userInput.isPressed(userInput.pttButton)
    if oldPTTpressed != currentPTTpressed:
        oldPTTpressed = currentPTTpressed
    
    
    # ctcss subtone
    if not userInput.memoryActive(): # memory channels use their own subtone
        vfo.setSubtoneOn(i2c.mode()==1)  # subtone on if mode is FM1
            
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
        
        
    # duplex offset
    if not userInput.memoryActive(): # memory channels use their own offset
        currentDuplexOffset = i2c.duplexOffset()
        if oldDuplexOffset != currentDuplexOffset:
            oldDuplexOffset = currentDuplexOffset
        if (currentDuplexOffset != 0) and not userInput.isPressed(userInput.pttButton):
            internalOutput.setTxForbidden(True)  # no TX allowed as long as PLL ist not configured
            
    
    # rotary encoder
    rotaryValue = encoder.getValueAndReset()
    if rotaryValue != 0:
        if not userInput.memoryActive():
            if i2c.mode()==1:    # FM1 = FM with subtone
                subtone.add(rotaryValue)
                vfo.setSubtoneIndex(subtone.getIndex())
            elif not userInput.isPressed(userInput.pttButton):  # encoder not in ctcss mode and ptt not pressed
                vfo.setRxFrequency(vfo.getRxFrequency() + rotaryValue*vfo.step())
        else: # memory active
            beep.beepError()


    # reverse button
    currentReverseMode = userInput.isPressed(userInput.reverseButton)
    if oldReverseMode != currentReverseMode:
        beep.beepOK()
        time.sleep_ms(_DEBOUNCE_TIME)
        oldReverseMode = currentReverseMode
        
    

        
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
        pllFrequency = currentFrequency - 1500
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
        i2c.dac.write(dacValue)
        
        internalOutput.setTxForbidden(False)
        
    # display line 1
    i2c.display.setFrequency(currentFrequency)
    

    # display offset
    if userInput.memoryActive():
        i2c.display.setOffset(vfo.getTxFrequency() - vfo.getRxFrequency())
    else: # transceiver in vfo mode
        i2c.display.setOffset(currentDuplexOffset)
    
    
    # display line 2
    if userInput.isPressed(userInput.msSwitch):
        i2c.display.setLine2("Memory Scan " + str(memoryScanChannel))
    elif userInput.isPressed(userInput.mrSwitch):
        i2c.display.setLine2("Memory " + str(i2c.memoryChannel()))
    else:
        i2c.display.setLine2("Step " + vfo.stepToString())

    # display line 3
    if scanner.isOn():
        i2c.display.setLine3("Scan")
    elif vfo.isSubtoneOn() and not userInput.isPressed(userInput.msSwitch):
        i2c.display.setLine3("CTCSS " + "{0:1}".format(subtone.value()))
    else:
        i2c.display.setLine3("");