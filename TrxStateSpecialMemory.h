#pragma once

#include "TrxState.h"

class TrxStateSpecialMemory : public TrxState
{
public:
    uint32_t getTxFrequency() const;
    bool isTxAllowed() const;
    bool isCtcssOn() const;
private:
    int32_t offset = 0;
};