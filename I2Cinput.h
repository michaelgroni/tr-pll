#pragma once

#include "settings.h"

class I2Cinput
{
public:
    static I2Cinput* getInstance();
    void update(); // must be called in the main loop

private:
    I2Cinput();                              
    I2Cinput(const I2Cinput&);                   // disable copy constructor              
    I2Cinput & operator = (const I2Cinput &);    // disable operator =
};