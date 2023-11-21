#pragma once

#include "pico/stdlib.h"

struct memory               // 16 bytes
{
    uint32_t rxFrequency;   // 4 bytes
    uint32_t txFrequency;   // 4 bytes
    uint8_t ctcssIndex;     // 1 byte
    bool ctcssOn;           // 1 byte
    uint32_t padding1;      // 4 bytes
    uint16_t padding2;      // 2 bytes
};

void flashInit();
uint32_t scanMin();
uint32_t scanMax();