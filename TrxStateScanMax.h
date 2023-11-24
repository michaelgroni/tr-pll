#pragma once

#include "TrxStateSpecialMemory.h"

#include "pico/stdlib.h"

class TrxStateScanMax: public TrxStateSpecialMemory
{
public:
    uint32_t getRxFrequency() const;
    void up(int n); 
    void save();   
};

inline TrxStateScanMax trxStateScanMax;