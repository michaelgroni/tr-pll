#pragma once

#include "pico/stdlib.h"

struct memory
{
    uint32_t rxFrequency;
    uint32_t txFrequency;
    size_t ctcssIndex;
    bool ctcssOn;
};

