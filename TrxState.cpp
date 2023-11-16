#include "TrxState.h"

#include "settings.h"
#include "GPIOinput.h"
#include "I2Cinput.h"

TrxState::TrxState(uint32_t rxFrequency)
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

uint32_t TrxState::getRxFrequency() const
{
    return rxFrequency;
}

uint32_t TrxState::getCurrentFrequency() const
{
    if (isPressed("ptt"))
    {
        return getTxFrequency();
    }
    else
    {
        return getRxFrequency();
    }
}


bool TrxState::isTxAllowed()
{
    auto const txf = getTxFrequency();
    return (txf >= F_MIN_TX) && (txf <= F_MAX_TX);
}

void TrxState::up(int n)
{
    auto mode = I2Cinput::getInstance()->getMode();

    if (mode != ctcss)
    {
        frequencyUp(n);
    }
}

void TrxState::stepUp()
{
    if (stepIndex == steps.size() - 1)
    {
        stepIndex = 0;
    }
    else
    {
        stepIndex++;
    }
}

void TrxState::stepDown()
{
    if (stepIndex == 0)
    {
        stepIndex = steps.size() -1;
    }
    else
    {
        stepIndex--;
    }
}

std::string TrxState::getStepToString() const
{
    if (stepIndex == 0)
    {
        return "auto";
    }
    else
    {
        return std::to_string(getStep());
    }
}

void TrxState::frequencyUp(int n)
{
    rxFrequency -= (rxFrequency % getStep()); // step could have been changed
    rxFrequency += (n * getStep());

    if (rxFrequency > F_MAX)
    {
        rxFrequency = F_MIN + rxFrequency - F_MAX - getStep();
    }
    else if (rxFrequency < F_MIN)
    {
        rxFrequency = F_MAX - (rxFrequency - F_MIN) + getStep();
    }
}

unsigned int TrxState::getStep() const
{
    if (stepIndex==0) // auto
    {
        auto mode = I2Cinput::getInstance()->getMode();

        if ((mode==fm2) || (mode==ctcss))
        {
            return 12500;
        }
        else
        {
            return 1000;
        }
    }
    else
    {
        return steps.at(stepIndex);
    }
}
