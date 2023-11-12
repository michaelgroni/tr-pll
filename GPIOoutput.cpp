#include "GPIOinput.h"

#include "settings.h"

void setTxAllowed(bool allowed)
{
    if (allowed)
    {
        gpio_put(TX_FORBIDDEN_PIN, 0);
    }
    else
    {
        gpio_put(TX_FORBIDDEN_PIN, 1);
    }
}