import _thread

class Scanner:
    __on = False
    __up = True
    
    __mutexOn = _thread.allocate_lock()
    __mutexUp = _thread.allocate_lock()

    def isOn(self):
        with self.__mutexOn:
            return self.__on
    
    def setOn(self, new_state):
        with self.__mutexOn:
            self.__on = new_state
        
    def isUp(self):
        with self.__mutexUp:
            return self.__up
    
    def setUp(self, new_state):
        with self.__mutexUp:
            self.__up = new_state

