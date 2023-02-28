class Subtone:
    
    __index=7
    __values = [67, 71.9, 74.4, 77, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100, 103.5, 107.2, 110.9, 114.8, 118.8, 123, 127.3, 131.8, 136.5,
           141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3]
    __changed = True
    
    
    def value(self):
        return self.__values[self.__index]
    
    def add(self, count):
        if count>=0:
            self.__index = min(self.__index + count, len(self.__values)-1)
        else:
            self.__index = max(self.__index + count, 0)
        self.__changed = True
        
    def getIndex(self):
        return self.__index
    
    def setIndex(self, index):
        self.__index = index
        self.__changed = True
                
    def changed(self):
        return self.__changed
                
    def changeProcessed(self):
        self.__changed = False
                
