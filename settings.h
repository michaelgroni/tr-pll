#pragma once

#include <unordered_map>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "hardware/flash.h"


// memory
inline const uint32_t MY_FLASH_OFFSET = 510 * FLASH_SECTOR_SIZE;
inline const uint32_t MY_FLASH_DATA = MY_FLASH_OFFSET + FLASH_SECTOR_SIZE;
inline const size_t MEMORIES = 100;


// frequencies
inline const uint32_t F_MIN = 143000000;
inline const uint32_t F_MAX = 150000000;
inline const uint32_t F_MIN_TX = 144000000;
inline const uint32_t F_MAX_TX = 146000000;
inline const uint32_t VFO_A_INIT = 145400000;
inline const uint32_t VFO_B_INIT = 144375000;


// waiting times in milliseconds
// inline const uint32_t DEBOUNCE_TIME = 200;
inline const uint32_t SCANNER_WAIT_QSO_TIME = 4000;
inline const uint32_t SCANNER_WAIT_PLL_TIME = 21;
inline const uint32_t SCANNER_WAIT_SQUELCH_TIME = 65;


// I2C bus
inline const uint8_t I2C_SDA = 0;
inline const uint8_t I2C_SCL = 1;
inline const uint I2C_CLOCK = 200000;
inline const auto I2C_PORT = i2c0;

// I2C OLED display 128 x 32
inline const int OLED_ADDRESS = 0x3C;
inline const bool OLED_FLIPPED = false;
inline const unsigned char OLED_CONTRAST = 255;

// I2C input / output
inline const uint8_t IC3_ADDR = 32;
inline const uint8_t IC4_ADDR = 33;

// I2C DAC (tunes drive unit)
inline const uint8_t DAC_ADDR = 96;
inline double dacVoltage(const uint32_t frequency) {return (frequency / 2000000.0) - 70.9;}

// SPI PLL
inline const auto SPI_PORT = spi0;
inline const uint8_t PLL_SPI_SCK = 6; // SCLK
inline const uint8_t PLL_SPI_TX = 7;  // MOSI
inline const uint8_t PLL_OUT_LE = 5;

// PIOs
inline const auto ROTARY_PIO = pio0;
inline const auto BEEP_PIO = pio0;
inline const auto CTCSS_PIO = pio1;

// GPIO input pins
inline const std::unordered_map<std::string, uint> gpioInPins =
{
    {"rotaryButton", 22},
    {"micUp", 12},
    {"micDown", 13},
    {"ptt", 20},
    {"stepDecrease", 17}, // formerly scan button
    {"stepIncrease", 16}, // formerly hold button
    {"m", 19}, // memory save button
    {"mr", 8}, // memory read switch
    {"ms", 9}, // memory scan swith
    {"ab", 11}, // pressed = VFO B - not pressed = VFO A
    {"reverse", 18},
    {"squelchOpen", 21}
};

// rotary encoder pio input pins
inline const uint8_t ROTARY_CLOCK = 14;
inline const uint8_t ROTARY_DATA = 15;

// GPIO output pin
inline const uint TX_FORBIDDEN_PIN = 2;

// pio output pins
inline const uint BEEP_PIN = 3;
inline const uint CTCSS_PIN = 10;