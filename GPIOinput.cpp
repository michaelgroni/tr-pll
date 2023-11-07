#include "GPIOinput.h"

GPIOinput::GPIOinput()
{}



GPIOinput* GPIOinput::getInstance()
{
    static GPIOinput instance;
    return &instance;
}

bool GPIOinput::isPressed(std::string pinName)
{
    return !gpio_get(gpioInPins[pinName]);
}
