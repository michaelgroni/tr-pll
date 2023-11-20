#include "GPIOinput.h"


bool isPressed(std::string pinName)
{
    auto pin = gpioInPins.at(pinName);
    return !gpio_get(pin);
}

bool wasPressed(std::string pinName)
{
    auto pin = gpioInPins.at(pinName);
    int32_t mask = 0xF << 4 * (pin % 8);
    uint32_t events = (iobank0_hw->intr[pin / 8] & mask) >> 4 * ( pin % 8);
    if (events & GPIO_IRQ_EDGE_FALL)
    {
        gpio_acknowledge_irq(pin, GPIO_IRQ_EDGE_FALL); // clear fall event
        return true;
    }
    else
    {
        return false;
    }
}

bool isSquelchOpen()
{
    auto pin = gpioInPins.at("squelchOpen");
    return gpio_get(pin);
}