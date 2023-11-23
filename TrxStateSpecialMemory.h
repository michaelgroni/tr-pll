#pragma once

#include "TrxState.h"

class TrxStateSpecialMemory : public TrxState
{
public:
    uint32_t getRxFrequency() const;
    uint32_t getTxFrequency() const;
    void up(int n);
    bool isTxAllowed() const;
    bool isCtcssOn() const;
protected:
    int32_t offset = 0;
};