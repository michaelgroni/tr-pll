#pragma once
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// frequencies
inline const uint32_t F_MIN = 143000000;
inline const uint32_t F_MAX = 150000000;


// I2C bus
inline const uint8_t I2C_SDA = 0;
inline const uint8_t I2C_SCL = 1;
inline const uint I2C_CLOCK = 200000;
inline const auto I2C_PORT = i2c0;

// I2C OLED display 128 x 32
inline const int OLED_ADDRESS = 0x3C;
inline const bool OLED_FLIPPED = false;

// I2C input / output
inline const uint8_t IC3_ADDR = 32;
inline const uint8_t IC4_ADDR = 33;