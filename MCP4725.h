#pragma once

#include "pico/stdlib.h"

void writeDAC(const uint16_t value);

uint16_t dacValue(const double voltage);
