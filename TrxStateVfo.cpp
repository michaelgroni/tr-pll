#include "TrxStateVfo.h"

#include "I2Cinput.h"
#include "GPIOinput.h"

TrxStateVfo::TrxStateVfo(uint32_t rxFrequency)
{
   setRxFrequency(rxFrequency);
}

void TrxStateVfo::setRxFrequency(uint32_t rxFrequency)
{
    if (rxFrequency < F_MIN)
    {
        this->rxFrequency = F_MIN;
    }
    else if (rxFrequency > F_MAX)
    {
        this->rxFrequency = F_MAX;
    }
    else
    {
        this->rxFrequency = rxFrequency;
    } 
}

uint32_t TrxStateVfo::getRxFrequency() const
{
    if (!isPressed("reverse"))
    {
        return rxFrequency;
    }
    else
    {
        auto offset = I2Cinput::getInstance()->getDuplexOffset();
        return rxFrequency + offset;
    }
}

uint32_t TrxStateVfo::getTxFrequency() const
{
    if (!isPressed("reverse"))
    {
        auto offset = I2Cinput::getInstance()->getDuplexOffset();
        return getRxFrequency() + offset;
    }
    else
    {
        return rxFrequency;
    }
}


bool TrxStateVfo::isCtcssOn() const
{
    return I2Cinput::getInstance()->getMode() == ctcss;
}


double TrxStateVfo::getCtcssFrequency() const
{
    return ctcssValues.at(ctcssIndex);
}


void TrxStateVfo::up(int n)
{
    auto mode = I2Cinput::getInstance()->getMode();

    if (mode != ctcss)
    {
        frequencyUp(n);
    }
    else // ctcss
    {
        ctcssUp(n);
    }
}

void TrxStateVfo::ctcssUp(int n)
{
    if (n>=0)
    {
        ctcssIndex = (ctcssIndex + n) % ctcssValues.size();
    }
    else // n<0
    {
        if (ctcssIndex >= -n)
        {
            ctcssIndex += n; 
        }
        else
        {
            ctcssIndex = ctcssValues.size() + n + ctcssIndex;
        }
    }
}


void TrxStateVfo::frequencyUp(int n)
{
    rxFrequency -= (rxFrequency % getStep()); // step could have been changed
    rxFrequency += (n * getStep());

    if (rxFrequency > F_MAX)
    {
        rxFrequency = F_MIN + rxFrequency - F_MAX - getStep();
    }
    else if (rxFrequency < F_MIN)
    {
        rxFrequency = F_MAX - (rxFrequency - F_MIN) - getStep();
    }
}
