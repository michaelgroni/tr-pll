#pragma once

#include "pico/stdlib.h"

class TrxState
{
public:
    TrxState(uint32_t rxFrequency);
    uint32_t getRxFrequency();

private:
    uint32_t rxFrequency;
};