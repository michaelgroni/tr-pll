#include "TrxState.h"

#include "settings.h"
#include "GPIOinput.h"
#include "I2Cinput.h"


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


bool TrxState::isTxAllowed()
{
    auto const txf = getTxFrequency();
    return (txf >= F_MIN_TX) && (txf <= F_MAX_TX);
}




