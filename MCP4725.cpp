#include "MCP4725.h"

#include "settings.h"


void writeDAC(const uint16_t value)
{
    static uint16_t oldValue = 0;

    if (value != oldValue)
    {
        const uint8_t byte2 = (value >> 8) & 0x0F;
        const uint8_t byte3 = (uint8_t) value;
        
        i2c_write_blocking(I2C_PORT, DAC_ADDR, &byte2, 1, true);
        i2c_write_blocking(I2C_PORT, DAC_ADDR, &byte3, 1, false);

        oldValue = value;
    }
}


uint16_t dacValue(const double voltage)
{
    return (uint16_t) (voltage / 3.3 * 4096);
}