#pragma once

#include "pico/stdlib.h"

#include <array>
#include <string>

#include "memory.h"

class TrxState
{
public:
    virtual uint32_t getRxFrequency() const = 0;
    virtual uint32_t getTxFrequency() const = 0;
    virtual void up(int n) = 0;
    virtual void stepUp() = 0;
    virtual void stepDown() = 0;

    uint32_t getCurrentFrequency() const;
    bool isTxAllowed() const;

    virtual bool isCtcssOn() const = 0;
    virtual double getCtcssFrequency() const = 0;

    memory toMemory() const;

protected:
    // ctcss frequencies
    const std::array<double, 38> ctcssValues = {67, 71.9, 74.4, 77, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100, 103.5,
        107.2, 110.9, 114.8, 118.8, 123, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9,
        186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3};
    size_t ctcssIndex = 20;
};