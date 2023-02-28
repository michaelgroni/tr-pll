import machine

squelchOpenInput = machine.Pin(21, machine.Pin.IN)

def isSquelchOpen():
    return squelchOpenInput.value()