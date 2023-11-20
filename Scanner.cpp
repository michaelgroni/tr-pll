#include "Scanner.h"

#include "hardware/timer.h"

#include "GPIOinput.h"
#include "GPIOoutput.h"

void Scanner::update(TrxStateVfo* trxStateVfo)
{
    sleep_ms(20); // wait for the pll

    if (wasOpen)
    {
        sleep_ms(60); // wait for the squelch to close
        wasOpen = false;
    }

    if (isSquelchOpen())
    {
        Piezo::getInstance()->beepOK();
        wasOpen = true;
        sleep_ms(SCANNER_WAIT_TIME);
    }

    if (up)
    {
        trxStateVfo->up(1);
    }
    else
    {
        trxStateVfo->up(-1);
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


bool Scanner::isOn()
{
    return on;
}