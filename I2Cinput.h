#pragma once

#include "settings.h"

enum mode {usb, lsb, fm1, ctcss};
// For the control unit CW ist the same as USB.
// The FM2 postion of the switch is used for CTCSS.

class I2Cinput
{
public:
    static I2Cinput* getInstance();
    void update(); // must be called in the main loop
    uint8_t getMemoryChannel();
    mode getMode();
    int32_t getDuplexOffset();

private:
    uint8_t byte1; // IC3
    uint8_t byte2; // IC4

    I2Cinput();                              
    I2Cinput(const I2Cinput&);                   // disable copy constructor              
    I2Cinput & operator = (const I2Cinput &);    // disable operator =
};