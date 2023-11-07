#include "GPIOinput.h"


bool isPressed(std::string pinName)
{
    auto pin = gpioInPins.at(pinName);
    return !gpio_get(pin);
}
