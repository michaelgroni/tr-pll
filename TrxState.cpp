#include "TrxState.h"


#include "settings.h"

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

uint32_t TrxState::getRxFrequency()
{
    return rxFrequency;
}
