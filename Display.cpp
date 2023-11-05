#include "Display.h"

#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "pico-ssd1306/shapeRenderer/ShapeRenderer.h"

Display::Display()
{
    oled.setOrientation(OLED_FLIPPED);
    drawText(&oled, font_12x16, "145400000", 0, 0);
    oled.sendBuffer();
    /*
    for (int y = 0; y < 32; y++)
    {
        oled.setPixel(64, y);
        oled.sendBuffer(); //Send buffer to device and show on screen
    }
    */
}


void Display::update()
{

}


Display* Display::getInstance()
{
    static Display instance;
    return &instance;
}


/*
    digits 0..9
    height 14, width 8
*/
void Display::drawCharacter(uint8_t x, uint8_t y, char c)
{
    switch (c)
    {
        case 1:
            drawLine(&oled, x+5, y, x+5, y+13);
            break;
        case 2:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+6);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x, y+8, x+6, y+8);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 3:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+13);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 4:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+8, y, x+13, y);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            break;
        case 6:
            drawLine(&oled, x, y+7, x, y+13);
        case 5:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y+7, x+8, y+13);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 0:
            drawLine(&oled, x, y, x, y+13);
            drawLine(&oled, x+1, y+14, x+7, y+14);
        case 7:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+13);
            break;
        case 8:
            drawLine(&oled, x, y+7, x, y+14);
        case 9:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+13);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
    default:  // unknown character
            drawLine(&oled, x+1, y, x+6, y);
            drawLine(&oled, x+1, y+7, x+6, y+14);
            drawLine(&oled, x+1, y+14, x+6, y+14); 
    }
}