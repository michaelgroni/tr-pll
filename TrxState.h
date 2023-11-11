#pragma once

#include "pico/stdlib.h"



class TrxState
{
public:
    TrxState(uint32_t rxFrequency);
    uint32_t getRxFrequency() const;
    virtual uint32_t getTxFrequency() const = 0;
    uint32_t getCurrentFrequency() const;
    bool isTxAllowed();
    void up(int n); // negative n for down


private:
    uint32_t rxFrequency;

    // available steps
    enum {A=0, B=10, C=100, D=1000, E=10000, F=12500, G=100000, H=1000000, Z=-1} step = B;
    // 0 means auto and -1 marks the last item.
};