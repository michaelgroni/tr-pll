import machine


class RotaryEncoder:
    __value = 0
    __state = 0         # 0, 1, 2 or 3
    
    def __init__(self, clockPin, dataPin):
        self.__clockPin = clockPin
        self.__dataPin = dataPin
        
        clockPin.irq(trigger=machine.Pin.IRQ_FALLING, handler=self.__encoderClockHandler)
        clockPin.irq(trigger=machine.Pin.IRQ_RISING, handler=self.__encoderClockHandler)
        dataPin.irq(trigger=machine.Pin.IRQ_FALLING, handler=self.__encoderDataHandler)
        dataPin.irq(trigger=machine.Pin.IRQ_RISING, handler=self.__encoderDataHandler)
 
        # 2-Bit-Gray-Code
        # 0 0   0 -> change value
        # 0 1   1
        # 1 1   2 -> change value
        # 1 0   3
        
    def __encoderClockHandler(self, p):
        irqState = machine.disable_irq()
        newState = self.__currentState()
        if (self.__state==3 and newState==0) or (self.__state==1 and newState==2):
            self.__value += 1
        self.__state = newState
        machine.enable_irq(irqState)


    def __encoderDataHandler(self, p):
        irqState = machine.disable_irq()
        newState = self.__currentState()
        if self.__state==1 and newState==0 or self.__state==3 and newState==2:
            self.__value -= 1
        self.__state = newState
        machine.enable_irq(irqState)
                
                    
    def __currentState(self):
        if self.__clockPin.value()==0:
            return (0 + self.__dataPin.value())
        else:
            return (3 - self.__dataPin.value())
        
        
    def getValue(self):
        return self.__value
        
    def up(self):
        self.__value += 1
        
    def down(self):
        self.__value -= 1
        
    def reset(self):
        self.__value = 0