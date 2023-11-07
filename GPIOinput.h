#pragma once

#include "settings.h"

#include <string>
class GPIOinput
{
public:
    static GPIOinput* getInstance();
    bool isPressed(std::string pinName);

private:
    GPIOinput();                              
    GPIOinput(const GPIOinput&);                   // disable copy constructor              
    GPIOinput & operator = (const GPIOinput &);    // disable operator =
};