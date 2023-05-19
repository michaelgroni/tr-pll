import machine
import time
import micropython as mp

class RotaryEncoder:

    
    def __init__(self, clockPin, dataPin):
        self.__value = 0
        
        self.__clockPin = clockPin
        self.__dataPin = dataPin
        
        clockPin.irq(trigger=machine.Pin.IRQ_RISING, handler=self.__encoderHandler, hard=True)
 
        
    def __encoderHandler(self, p):
        if self.__clockPin.value() == 1:
            if self.__dataPin.value() == 1:
                self.down()
            else:
                self.up()
        
        
    def getValueAndReset(self):
        value=self.__value
        self.__value = 0
        return value
        
    def up(self):
        self.__value += 1
        
    def down(self):
        self.__value -= 1
