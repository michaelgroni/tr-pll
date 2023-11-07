#include "GPIOinput.h"

GPIOinput::GPIOinput()
{

}



GPIOinput* GPIOinput::getInstance()
{
    static GPIOinput instance;
    return &instance;
}
