#pragma once

#include "pico/stdlib.h"

struct memory               // 16 bytes
{
    uint32_t rxFrequency;   // 4 bytes
    uint32_t txFrequency;   // 4 bytes
    uint8_t ctcssIndex;     // 1 byte
    bool ctcssOn;           // 1 byte
    bool isUsed;            // 1 byte
    uint32_t padding1;      // 4 bytes
    uint8_t padding2;       // 1 byte
};

void flashInit();
uint32_t scanMin();
uint32_t scanMax();
void saveScanMin(uint32_t frequency);
void saveScanMax(uint32_t frequency);