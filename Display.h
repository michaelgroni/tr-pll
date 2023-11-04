#pragma once

#include "pico-ssd1306/ssd1306.h"

#include <memory>

#include "settings.h"

class Display
{
public:
    static Display* getInstance()
    {
        static Display instance;
        return &instance;
    }

private:
    Display();                              
    Display(const Display&);                   // disable copy constructor              
    Display & operator = (const Display &);    // disable operator =

    pico_ssd1306::SSD1306 oled = pico_ssd1306::SSD1306(I2C_PORT, OLED_ADDRESS, pico_ssd1306::Size::W128xH32);
};