#pragma once

#include "TrxState.h"

class TrxStateMemories : public TrxState
{
public:
    TrxStateMemories();
    uint32_t getRxFrequency() const;
    uint32_t getTxFrequency() const;
    void up(int n);
    void stepUp();
    void stepDown();
    bool isCtcssOn() const;
    double getCtcssFrequency() const;
private:
    size_t memoryIndex; // 1 .. MEMORIES
};