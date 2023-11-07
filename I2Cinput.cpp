#include "I2Cinput.h"

I2Cinput::I2Cinput()
{

}


void I2Cinput::update() // must be called in the main loop
{
    i2c_read_blocking(I2C_PORT, IC3_ADDR, &byte1, 1, false);
    i2c_read_blocking(I2C_PORT, IC4_ADDR, &byte2, 1, false);
}

uint8_t I2Cinput::getMemoryChannel() // memory switch
{
    if (byte2 & 4 == 0)
        return 6;
    if (byte2 & 8 == 0)
        return 5;
    if (byte1 & 1 == 0)
        return 4;
    if (byte1 & 2 == 0)
        return 3;
    if (byte1 & 4 == 0)
        return 2;
    return 1;
}

mode I2Cinput::getMode()
{
    if (byte1 & 8 == 0)
        return lsb;
    if (byte1 & 16 == 0)
        return fm1;
    if (byte1 & 32 == 0)
        return ctcss;
    return usb;
}

int32_t I2Cinput::getDuplexOffset()
{
    mode m = getMode();
    if (m==fm1 || m==ctcss)
    {    
        if (byte1 & 64 == 0)
            return -600000;
        if (byte1 & 128 == 0)
            return 600000;
        return 0;
    }
    else // not FM
    {
        return 0;
    }
}

I2Cinput* I2Cinput::getInstance()
{
    static I2Cinput instance;
    return &instance;
}
