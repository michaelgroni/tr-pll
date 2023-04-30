import machine
import time
import _thread
import micropython as mp

_ROTARY_DEBOUNCE_TIME = const(150) # milliseconds

class RotaryEncoder:

    
    def __init__(self, clockPin, dataPin):
        self.__value = 0
        #self.__state = 0         # 0, 1, 2 or 3
        
        self.__clockPin = clockPin
        self.__dataPin = dataPin
        
        self.__oldClock = clockPin.value()
        
        self.__valueLock = _thread.allocate_lock()
        self.__inpuLock = _thread.allocate_lock()
        
        #clockPin.irq(trigger=machine.Pin.IRQ_FALLING | machine.Pin.IRQ_RISING, handler=self.__encoderHandler)
        clockPin.irq(trigger=machine.Pin.IRQ_RISING, handler=self.__encoderHandler)
 
        # 2-Bit-Gray-Code
        # 0 0   0 -> change value
        # 0 1   1
        # 1 1   2 -> change value
        # 1 0   3
        
    def __encoderHandler(self, p):            
        if self.__dataPin.value() == 1:
            self.up()
        else:
            self.down()
  
                    
    #def __currentState(self):
        #if self.__clockPin.value()==0:
            #return (self.__dataPin.value())
        #else:
            #return (3 - self.__dataPin.value())
        
        
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