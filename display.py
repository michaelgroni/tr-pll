import machine
import numbers
import ssd1306

class Display:
    __oldLine2 = ""
    __oldLine3 = ""
    
    def __init__(self, i2c):
        self.__oled = ssd1306.SSD1306_I2C(128, 32, i2c, 0x3c, False)
        self.__oled.fill(0)  # black
    
    def setFrequency(self, frequency):
        self.__oled.fill_rect(0, 0, 110, 15, 0)  # overwrite old content
        x=0
        column=0
        for n in str(frequency):
            numbers.draw(self.__oled, x, 0, int(n))
            if column==2 or column==5:
                self.__oled.pixel(x+11, 14, 0xffff)
                x+=14
            else:
                x+=12
            column+=1
        self.__oled.show()
        
    
    def setOffset(self, offset):
        self.__oled.fill_rect(120, 0, 127, 15, 0)  # overwrite old content
        offsetString = " "
        if offset > 0:
            offsetString = "+"
        elif offset < 0:
            offsetString = "-"
        self.__oled.text(offsetString, 120, 4)
        self.__oled.show()
    
    def setLine2(self, s):
        if s != self.__oldLine2:
            self.__oldLine2 = s
            self.__oled.fill_rect(0 , 16, 119, 9, 0)  # overwrite old content
            self.__oled.text(s, 1, 16)
            self.__oled.show()
    
    def setLine3(self, s):
        if s != self.__oldLine3:
            self.__oldLine3 = s
            self.__oled.fill_rect(0 , 25, 127, 9, 0)  # overwrite old content
            self.__oled.text(s, 1, 25)
            self.__oled.show()