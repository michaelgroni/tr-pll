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



bool TrxState::isTxAllowed() const
{
    auto const txf = getTxFrequency();
    return (txf >= F_MIN_TX) && (txf <= F_MAX_TX);
}


memory TrxState::toMemory() const
{
    struct memory m;
    m.rxFrequency = getRxFrequency();
    m.txFrequency = getTxFrequency();
    m.ctcssIndex = ctcssIndex;
    m.ctcssOn = isCtcssOn();
    return m;
}




