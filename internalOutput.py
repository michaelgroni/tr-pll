import machine
import time

# tx forbidden

txForbiddenOutput = machine.Pin(2, machine.Pin.OUT)

def setTxForbidden(on):
    txForbiddenOutput.value(on)
    
    
# pll

spiSck = machine.Pin(6)
spiTx = machine.Pin(7)
outLE = machine.Pin(5, mode=machine.Pin.OUT, value=1)

spi=machine.SPI(0,baudrate=1000000,sck=spiSck, mosi=spiTx)

def writePLL(spi, values):
    outLE(0)
    spi.write(values)
    outLE(1)
    time.sleep_us(1)
    
def initPLL():
    # 145400000
    # R5
    writePLL(spi, bytearray(b'\x00\x58\x00\x05'))
    # R4
    writePLL(spi, bytearray(b'\x00\xD0\x14\x3C'))
    # R3
    writePLL(spi, bytearray(b'\x00\x80\x04\xB3'))
    # R2
    writePLL(spi, bytearray(b'\x00\x06\x4E\x42'))
    # R1
    writePLL(spi, bytearray(b'\x08\x00\x80\x29'))
    # R0
    writePLL(spi, bytearray(b'\x15\x0C\x00\x10'))
