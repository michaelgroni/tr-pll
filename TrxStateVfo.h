#pragma once

#include "pico/stdlib.h"

#include "TrxState.h"

class TrxStateVfo : public TrxState
{
public:
    TrxStateVfo(uint32_t rxFrequency);
    uint32_t getRxFrequency() const;
    void setRxFrequency(uint32_t frequency);
    uint32_t getTxFrequency() const;
    bool isCtcssOn() const;
    double getCtcssFrequency() const;
    void up(int n);
    void stepUp();
    void stepDown();
    std::string getStepToString() const;

private:
    uint32_t rxFrequency;

    void frequencyUp(int n); // negative n for down
    void ctcssUp(int n); // negative n for down

    // available steps
    const std::array<unsigned int, 8> steps = {0, 10, 100, 1000, 10000, 12500, 100000, 1000000}; // 0 means auto
    size_t stepIndex = 0; // auto

    unsigned int getStep() const;
};