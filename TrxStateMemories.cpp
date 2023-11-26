#include "TrxStateMemories.h"

#include "settings.h"
#include "memory.h"
#include "GPIOoutput.h"

TrxStateMemories::TrxStateMemories()
{
    // find a used memory channel
    memoryIndex = MEMORIES;
    for (size_t i = 1; (i<=MEMORIES) && (memoryIndex == MEMORIES); i++)
    {
        if (flashData[i].isUsed)
        {
            memoryIndex = i;
        }
    }
}

uint32_t TrxStateMemories::getRxFrequency() const
{
    if  (flashData[memoryIndex].isUsed)
    {
        return flashData[memoryIndex].rxFrequency;
    }
    else
    {
        return 0;
    }
}

uint32_t TrxStateMemories::getTxFrequency() const
{
    if  (flashData[memoryIndex].isUsed)
    {
        return flashData[memoryIndex].txFrequency;
    }
    else
    {
        return 0;
    }
}

void TrxStateMemories::up(int n)
{
    int step = 0;
    if (n>0)
    {
        step = 1;
    }
    else if (n<0)
    {
        step = -1;
    }

    auto oldMemoryIndex = memoryIndex;

    do
    {
        if ((memoryIndex == MEMORIES) && (step == 1))
        {
            memoryIndex = 1;
        }
        else if ((memoryIndex == 1) && (step == -1))
        {
            memoryIndex = MEMORIES;
        }
        else
        {
            memoryIndex += step;
        }

        if (flashData[memoryIndex].isUsed || isWriteModeOn())
        {
            n -= step;
        }
    } while ((n != 0) && (memoryIndex != oldMemoryIndex));
}

size_t TrxStateMemories::getMemoryIndex() const
{
    return memoryIndex;
}

void TrxStateMemories::stepUp()
{
    Piezo::getInstance()->beepError();
}

void TrxStateMemories::stepDown()
{
    Piezo::getInstance()->beepError();
}

bool TrxStateMemories::isCtcssOn() const
{
    return flashData[memoryIndex].isCtcssOn;
}

double TrxStateMemories::getCtcssFrequency() const
{
    return ctcssValues.at(flashData[memoryIndex].ctcssIndex);
}

bool TrxStateMemories::isWriteModeOn() const
{
    return writeModeOn;
}

void TrxStateMemories::setWriteModeOn(bool writeModeOn)
{
    this->writeModeOn = writeModeOn;
}