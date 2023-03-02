import time
import machine
from rp2 import PIO, StateMachine, asm_pio

# beep pio
@asm_pio(set_init=PIO.OUT_LOW)
def beep():
    set(pins, 1) [2]
    set(pins, 0) [2]
beepMachine = StateMachine(1, beep, freq=2000, set_base=machine.Pin(3))
beepMachine.active(0)

def beepOK():
    beepMachine.init(beep, freq=10000, set_base=machine.Pin(3))
    beepMachine.active(1)
    time.sleep_ms(75)
    beepMachine.active(0)
    
def beepWriteOK():
    beepMachine.init(beep, freq=8000, set_base=machine.Pin(3))
    beepMachine.active(1)
    time.sleep_ms(250)
    beepMachine.active(0)
    
def beepError():
    for i in range(4):
        beepMachine.init(beep, freq=2000, set_base=machine.Pin(3))
        beepMachine.active(1)
        time.sleep_ms(50)
        beepMachine.active(0)
        time.sleep_ms(50)