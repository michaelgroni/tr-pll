#pragma once

#include "TrxStateSpecialMemory.h"

class TrxStateScanMin : public TrxStateSpecialMemory
{
public:
    uint32_t getRxFrequency() const;
    void up(int n);
};