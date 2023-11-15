#pragma once

#include "pico/stdlib.h"

#include "TrxState.h"

class TrxStateVfo : public TrxState
{
public:
    TrxStateVfo(uint32_t rxFrequency);
    uint32_t getTxFrequency() const;
    bool isCtcssOn() const;
private:
};