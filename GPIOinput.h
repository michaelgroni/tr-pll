#pragma once

#include "settings.h"

class GPIOinput
{
public:
    static GPIOinput* getInstance();

private:
    GPIOinput();                              
    GPIOinput(const GPIOinput&);                   // disable copy constructor              
    GPIOinput & operator = (const GPIOinput &);    // disable operator =
};