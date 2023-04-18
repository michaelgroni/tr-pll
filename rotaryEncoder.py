import machine
import time
import _thread
import micropython as mp


class RotaryEncoder:

    
    def __init__(self, clockPin, dataPin):
        self.__value = 0
        self.__state = 0         # 0, 1, 2 or 3
        
        self.__clockPin = clockPin
        self.__dataPin = dataPin
        
        self.__valueLock = _thread.allocate_lock()
        
        clockPin.irq(trigger=machine.Pin.IRQ_FALLING | machine.Pin.IRQ_RISING, handler=self.__encoderHandler)
        dataPin.irq(trigger=machine.Pin.IRQ_FALLING | machine.Pin.IRQ_RISING, handler=self.__encoderHandler)
 
        # 2-Bit-Gray-Code
        # 0 0   0 -> change value
        # 0 1   1
        # 1 1   2 -> change value
        # 1 0   3
        
    def __encoderHandler(self, p):
        irqState = machine.disable_irq()
        newState = self.__currentState()
        if (self.__state==3 and newState==0) or (self.__state==1 and newState==2):
            self.up()
        elif (self.__state==1 and newState==0) or (self.__state==3 and newState==2):
            self.down()
        self.__state = newState
        machine.enable_irq(irqState)
  
                    
    def __currentState(self):
        if self.__clockPin.value()==0:
            return (self.__dataPin.value())
        else:
            return (3 - self.__dataPin.value())
        
        
    def getValueAndReset(self):
        with self.__valueLock:
            value=self.__value
            self.__value = 0
        return value
        
    def up(self):
        with self.__valueLock:
            self.__value += 1
        
    def down(self):
        with self.__valueLock:
            self.__value -= 1