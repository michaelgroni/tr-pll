#pragma once

#include <unordered_map>

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

// GPIO input pins
inline const std::unordered_map<std::string, uint> gpioInPins =
{
    {"rotaryClock", 14},
    {"rotaryData", 15},
    {"rotaryButton", 22},
    {"micUp", 12},
    {"micDown", 13},
    {"ptt", 20},
    {"stepDecrease", 17}, // formerly scan button
    {"stepIncrease", 16}, // formerly hold button
    {"mr", 8}, // memory read switch
    {"ms", 9}, // memory scan swith
    {"ab", 11}, // pressed = VFO B - not pressed = VFO A
    {"reverse", 18}
};