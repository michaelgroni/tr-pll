#include "Display.h"

#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "pico-ssd1306/shapeRenderer/ShapeRenderer.h"


Display::Display()
{
    oled.setOrientation(OLED_FLIPPED);
    oled.setContrast(OLED_CONTRAST);
}


void Display::update(const TrxState& trxState)
{
    bool changed = false;

    
    // frequency
    auto newFrequency = trxState.getCurrentFrequency();
    if (newFrequency != frequency)
    {
        setFrequency(newFrequency);
        changed = true;
    }


    // duplex shift
    const auto txf = trxState.getTxFrequency();
    const auto rxf = trxState.getRxFrequency();
    char infoNortheast;

    if (txf < rxf)
    {
        infoNortheast = '-';
    }
    else if (txf > rxf)
    {
        infoNortheast = '+';
    }
    else
    {
        infoNortheast = ' ';
    }

    if (infoNortheast != this->infoNortheast)
    {
        setInfoNortheast(infoNortheast);
        changed = true;
    }


    // step
    auto newStep = trxState.getStepToString();
    if (step.compare(newStep) != 0)
    {
        setStep(newStep);
        changed = true;
    }


    if (changed)
    {
        oled.sendBuffer();
    }
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
void Display::drawDigit(uint8_t x, uint8_t y, uint digit)
{
    switch (digit)
    {
        case 1:
            drawLine(&oled, x+5, y, x+5, y+14);
            break;
        case 2:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+6);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x, y+8, x, y+13);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 3:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+14);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 4:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+8, y, x+8, y+14);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            break;
        case 6:
            drawLine(&oled, x, y+7, x, y+14);
        case 5:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y+7, x+8, y+14);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
        case 0:
            drawLine(&oled, x, y, x, y+14);
            drawLine(&oled, x+1, y+14, x+7, y+14);
        case 7:
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+14);
            break;
        case 8:
            drawLine(&oled, x, y+7, x, y+14);
        case 9:
            drawLine(&oled, x, y, x, y+6);
            drawLine(&oled, x+1, y, x+7, y);
            drawLine(&oled, x+8, y, x+8, y+14);
            drawLine(&oled, x+1, y+7, x+7, y+7);
            drawLine(&oled, x+1, y+14, x+7, y+14);
            break;
    default:  // unknown character
            drawLine(&oled, x+1, y, x+6, y);
            drawLine(&oled, x+1, y+7, x+6, y+14);
            drawLine(&oled, x+1, y+14, x+6, y+14); 
    }
}

void Display::setFrequency(uint32_t frequency)
{
    this->frequency = frequency;

    fillRect(&oled, 0, 0, 110, 15, pico_ssd1306::WriteMode::SUBTRACT); // overwrite old content

    uint8_t x=0;    
    uint column=0;
    for (uint i = 1000000000; i>=10; i/=10)
    {
        uint digit = (frequency % i) / (i/10);
        drawDigit(x, 0, digit);

        if (column==2 || column==5) // thousands point
        {
            oled.setPixel(x+11, 14);
            x += 14;
        }
        else // no thousands point
        {
            x += 12;
        }

        column++;
    }
}

void Display::setStep(const std::string step)
{
    this->step = step;

    fillRect(&oled, 0, 17, 128, 24, pico_ssd1306::WriteMode::SUBTRACT); // overwrite old content
    std::string s = "Step " + step;
    drawText(&oled, font_8x8, s.c_str(), 0, 17);
}

void Display::setInfoNortheast(const char c)
{
    this->infoNortheast = c;

    char cString[2];
    cString[0] = c;
    cString[1] = '\0';

    fillRect(&oled, 117, 0, 127, 15, pico_ssd1306::WriteMode::SUBTRACT);  // overwrite old content
    drawText(&oled, font_12x16, cString, 117, 0);
}
