import machine
import _thread

import i2c

class Vfo:
    # frequency step
    __index=0
    __values = [0, 10, 100, 1000, 10000, 12500, 100000, 1000000] # 0 means auto
    
    
    def __init__(self, new_rxFrequency, new_txFrequency, new_subtoneIndex, subtoneOn):
        self.__rxFrequency = new_rxFrequency
        self.__txFrequency = new_txFrequency
        self.__subtoneIndex = new_subtoneIndex
        self.__subtoneOn = subtoneOn

    def getRxFrequency(self):
        return self.__rxFrequency
       
    def setRxFrequency(self, new_frequency):
        self.__rxFrequency = new_frequency
        
    def getTxFrequency(self):
        return self.__txFrequency
    
    def setTxFrequency(self, new_frequency):
        self.__txFrequency = new_frequency
        
    def getSubtoneIndex(self):
        return self.__subtoneIndex
    
    def setSubtoneIndex(self, new_subtoneIndex):
        self.__subtoneIndex = new_subtoneIndex
           
    def isSubtoneOn(self):
        return self.__subtoneOn
    
    def setSubtoneOn(self, subtoneOn):
        self.__subtoneOn = subtoneOn        
    
    def step(self):
        if self.__index>0:  # not auto
            currentStep = self.__values[self.__index]
        else:               # auto
            currentStep = self.__autoStep()
    
        self.__rxFrequency -= self.__rxFrequency % currentStep
        self.__txFrequency -= self.__txFrequency % currentStep
        
        return currentStep
            
    def __autoStep(self):
        if i2c.mode()<3:      # FM
            return 12500
        else:           # SSB, CW
            return 1000        
            
    def stepToString(self):
        if self.__index==0:
            return "auto"
        elif self.step()<1000:
            return str(self.step()) + " Hz"
        elif self.step()<1000000:
            return str(self.step()/1000) + " kHz"
        else:
            return str(self.step()/1000000) + " MHz"

    def incStep(self, count):
        self.__index = self.__index + count
        while self.__index<0:
            self.__index = self.__index + len(self.__values)
        self.__index = self.__index % len(self.__values) 
            
    def toCsvString(self):
        return str(self.getRxFrequency()) + ";" + str(self.getTxFrequency()) + ";" + str(self.getSubtoneIndex()) + ";" + str(self.isSubtoneOn())
        
