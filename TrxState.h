#pragma once

#include "pico/stdlib.h"

#include <array>

class TrxState
{
public:
    TrxState(uint32_t rxFrequency);

    uint32_t getRxFrequency() const;
    virtual uint32_t getTxFrequency() const = 0;
    uint32_t getCurrentFrequency() const;

    bool isTxAllowed();

    void up(int n); // negative n for down

    void stepUp();
    void stepDown();
    unsigned int getStep();


private:
    uint32_t rxFrequency;

    // available steps
    std::array<unsigned int, 8> steps = {0, 10, 100, 1000, 10000, 12500, 100000, 1000000}; // 0 means auto
    size_t stepIndex = 0; // auto
};