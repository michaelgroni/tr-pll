#pragma once

#include "pico/stdlib.h"

class TrxState
{
public:
    TrxState(uint32_t rxFrequency);
    uint32_t getRxFrequency() const;
    virtual uint32_t getTxFrequency() const = 0;
    uint32_t getCurrentFrequency() const;
    void setRxFrequency(uint32_t frequency);
    bool isTxAllowed();

private:
    uint32_t rxFrequency;
};