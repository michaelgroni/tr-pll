#include "Display.h"

Display::Display()
{
    for (int y = 0; y < 32; y++)
    {
        oled.setPixel(64, y);
        oled.sendBuffer(); //Send buffer to device and show on screen
    }
}
