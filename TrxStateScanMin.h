#pragma once

#include "TrxStateSpecialMemory.h"

#include "pico/stdlib.h"

class TrxStateScanMin : public TrxStateSpecialMemory
{
public:
    uint32_t getRxFrequency() const;
    void up(int n); 
    void save();   
};

inline TrxStateScanMin trxStateScanMin;