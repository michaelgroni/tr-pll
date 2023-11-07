#include "GPIOinput.h"


bool isPressed(std::string pinName)
{
    return !gpio_get(gpioInPins.at(pinName));
}
