#include "I2Cinput.h"

I2Cinput::I2Cinput()
{

}


void I2Cinput::update() // must be called in the main loop
{

}


I2Cinput* I2Cinput::getInstance()
{
    static I2Cinput instance;
    return &instance;
}
