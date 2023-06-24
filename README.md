# tr-pll
## Modifications for the transceiver Kenwood TR-9130
* new control board with a Raspberry Pi Pico
* OLED display
* new PLL with TCXO, step size 10 Hz
* CTCSS encoder
* memory channels without backup battery

## Circuit diagrams with notes in German
### CONTROL UNIT
![CONTROL UNIT](CONTROL UNIT v2.2.TIF)

### PLL UNIT
![PLL UNIT](PLL UNIT.png)

### OPTIONAL (notes in German)
![OPTIONAL](circuit2.png)

## Installation on a Raspberry Pi Pico
1. Follow the instructions on https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico to learn how to load the MicroPython firmware and Micropython code on a Raspberry Pi Pico.
2. Upload all files from this repository that end with .py on the Pico. If necessary, adjust the addresses of the display and the digital-to-analogue converter on the I²C bus in the internalOutput.py file before uploading.

## Coming soon: new release
* PCB layout
* all inputs digital
