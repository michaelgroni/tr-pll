#include "I2Cinput.h"

#include "hardware/timer.h"

I2Cinput::I2Cinput()
{}


void I2Cinput::update() // must be called in the main loop
{
    i2c_read_blocking(I2C_PORT, IC3_ADDR, &byte1, 1, false);
    i2c_read_blocking(I2C_PORT, IC4_ADDR, &byte2, 1, false);
}

uint8_t I2Cinput::getSpecialMemoryChannel() // memory switch
{
    if ((byte2 & 4) == 0)
        return 6;
    if ((byte2 & 8) == 0)
        return 5;
    if ((byte1 & 1) == 0)
        return 4;
    if ((byte1 & 2) == 0)
        return 3;
    if ((byte1 & 4) == 0)
        return 2;
    return 1;
}

mode I2Cinput::getMode()
{
    mode m = getModePrivate();

    if (m == usb) // mode is USB/CW or the switch is being moved at the moment
    {
        sleep_ms(6);
        i2c_read_blocking(I2C_PORT, IC3_ADDR, &byte1, 1, false);
        return getModePrivate();
    }
    else
    {
        return m;
    }
}

mode I2Cinput::getModePrivate()
{
    if ((byte1 & 8) == 0)
        return lsb;
    if ((byte1 & 16) == 0)
        return ctcss;
    if ((byte1 & 32) == 0)
        return fm2;
    return usb;
}

int32_t I2Cinput::getDuplexOffset()
{
    mode m = getMode();

    if ((m==fm2) || (m==ctcss))
    {    
        if ((byte1 & 64) == 0)
            return -600000;
        if ((byte1 & 128) == 0)
            return 600000;
    }
    
    return 0;
}

I2Cinput* I2Cinput::getInstance()
{
    static I2Cinput instance;
    return &instance;
}
