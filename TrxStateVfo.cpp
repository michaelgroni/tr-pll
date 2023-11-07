#include "TrxStateVfo.h"

#include "I2Cinput.h"


TrxStateVfo::TrxStateVfo(uint32_t rxFrequency)
: TrxState(rxFrequency)
{
}


uint32_t TrxStateVfo::getTxFrequency() const
{
    auto offset = I2Cinput::getInstance()->getDuplexOffset();
    return getRxFrequency() + offset;
}
