#include "TrxState.h"

#include "settings.h"
#include "GPIOinput.h"

TrxState::TrxState(uint32_t rxFrequency)
{
    if (rxFrequency < F_MIN)
    {
        this->rxFrequency = F_MIN;
    }
    else if (rxFrequency > F_MAX)
    {
        this->rxFrequency = F_MAX;
    }
    else
    {
        this->rxFrequency = rxFrequency;
    }
}

uint32_t TrxState::getRxFrequency() const
{
    return rxFrequency;
}

uint32_t TrxState::getCurrentFrequency() const
{
    if (isPressed("ptt"))
    {
        return getTxFrequency();
    }
    else
    {
        return getRxFrequency();
    }
}