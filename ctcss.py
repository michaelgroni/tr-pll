import machine
from rp2 import PIO, StateMachine, asm_pio

# ctcss pio    
@asm_pio(set_init=PIO.OUT_LOW)
def ctcss():         # PWM
    set(pins, 1) [4] # 0°
    set(pins, 0) [4]
    
    set(pins, 1) [6] # 20°
    set(pins, 0) [2]

    set(pins, 1) [7] # 40°
    set(pins, 0) [1]

    set(pins, 1) [8] # 60°
    set(pins, 0)
    
    set(pins, 1) [19] # 80° + 100°   
    
    set(pins, 1) [8] # 120°
    set(pins, 0)
    
    set(pins, 1) [7] # 140°
    set(pins, 0) [1]
    
    set(pins, 1) [6] # 160°
    set(pins, 0) [2]
        
    set(pins, 1) [4] # 180°
    set(pins, 0) [4]

    set(pins, 1) [2] # 200°
    set(pins, 0) [6]
    
    set(pins, 1) [1] # 220°
    set(pins, 0) [7]
    
    set(pins, 1)     # 240°
    set(pins, 0) [8]
    
    set(pins, 0) [19] # 260° + 280°
    
    set(pins, 1)     # 300°
    set(pins, 0) [8]
    
    set(pins, 1) [1] # 320°
    set(pins, 0) [7]
    
    set(pins, 1) [2] # 340°
    set(pins, 0) [6]

ctcssMachine = StateMachine(4, ctcss, freq=13275, set_base=machine.Pin(10))
ctcssMachine.active(0)
