#pragma once

#include "TrxState.h"

class TrxStateSpecialMemory : public TrxState
{
public:
    uint32_t getTxFrequency() const;
    bool isTxAllowed() const;
    bool isCtcssOn() const;
    bool isNotSaved() const;
protected:
    int32_t offset = 0;
};