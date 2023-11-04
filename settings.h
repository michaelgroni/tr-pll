#pragma once
#include "pico/stdlib.h"

// I2C
inline const uint8_t I2C_SDA = 0;
inline const uint8_t I2C_SCL = 1;
inline const uint I2C_CLOCK = 200000;
inline const auto I2C_PORT = i2c0;