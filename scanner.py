class Scanner:
    __on = False
    __up = True

    def isOn(self):
        return self.__on
    
    def setOn(self, new_state):
        self.__on = new_state
        
    def isUp(self):
        return self.__up
    
    def setUp(self, new_state):
        self.__up = new_state

