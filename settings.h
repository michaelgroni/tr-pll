#pragma once
#include "pico/stdlib.h"

// I2C bus
inline const uint8_t I2C_SDA = 0;
inline const uint8_t I2C_SCL = 1;
inline const uint I2C_CLOCK = 200000;
inline const auto I2C_PORT = i2c0;

// I2C OLED display 128 x 32
inline const int OLED_ADDRESS = 0x3C;