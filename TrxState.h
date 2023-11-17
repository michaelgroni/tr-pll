#pragma once

#include "pico/stdlib.h"

#include <array>
#include <string>

class TrxState
{
public:
    virtual uint32_t getRxFrequency() const = 0;
    virtual uint32_t getTxFrequency() const = 0;
    virtual void up(int n) = 0;
    virtual void stepUp() = 0;
    virtual void stepDown() = 0;

    uint32_t getCurrentFrequency() const;

    bool isTxAllowed();

    virtual bool isCtcssOn() const = 0;
    virtual double getCtcssFrequency() const = 0;

private:
};