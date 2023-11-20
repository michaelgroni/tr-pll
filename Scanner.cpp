#include "Scanner.h"

#include "hardware/timer.h"

#include "GPIOinput.h"
#include "GPIOoutput.h"


void Scanner::update(TrxStateVfo *trxStateVfo)
{
    sleep_ms(SCANNER_WAIT_PLL_TIME); // wait for the pll

    if (wasOpen)
    {
        sleep_ms(SCANNER_WAIT_SQUELCH_TIME); // wait for the squelch to close
        wasOpen = false;
    }

    if (isSquelchOpen())
    {
        Piezo::getInstance()->beepOK();
        wasOpen = true;
        for (int i = 0; (i < 10) && isOn(); i++)
        {
            sleep_ms(SCANNER_WAIT_QSO_TIME / 10);
            if (wasPressed("ptt") || wasPressed("rotaryButton"))
            {
                Piezo::getInstance()->beepOK();
                setOn(false);
            }
        }
    }

    if (isOn())
    {
        if (up)
        {
            trxStateVfo->up(1);
        }
        else
        {
            trxStateVfo->up(-1);
        }
    }
}

void Scanner::setUp(bool up)
{
    this->up = up;
}

void Scanner::setOn(bool on)
{
    this->on = on;
}

bool Scanner::isOn() const
{
    return on;
}