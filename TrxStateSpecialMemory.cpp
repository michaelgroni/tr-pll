#include "TrxStateSpecialMemory.h"


uint32_t TrxStateSpecialMemory::getTxFrequency() const
{
    return getRxFrequency();
}

bool TrxStateSpecialMemory::isTxAllowed() const
{
    return false;
}

bool TrxStateSpecialMemory::isCtcssOn() const
{
    return false;
}

bool TrxStateSpecialMemory::isNotSaved() const
{
    return offset != 0;
}