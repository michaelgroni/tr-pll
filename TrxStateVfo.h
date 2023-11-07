#pragma once

#include "pico/stdlib.h"

#include "TrxState.h"

class TrxStateVfo : public TrxState
{
public:
    TrxStateVfo(uint32_t rxFrequency);

private:
};