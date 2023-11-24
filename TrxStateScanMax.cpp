#include "TrxStateScanMax.h"

#include "settings.h"

uint32_t TrxStateScanMax::getRxFrequency() const
{
    return scanMax() + offset;
}

void TrxStateScanMax::up(int n)
{
    while ( getRxFrequency() % getStep() != 0) // step could have been changed
    {
        offset--;
    }

    offset += n*getStep();

    if (getRxFrequency() > F_MAX)
    {
        offset = F_MAX - scanMax();
    }
    else if (getRxFrequency() < F_MIN)
    {
        offset = F_MIN - scanMin();
    }
}

void TrxStateScanMax::save()
{
    saveScanMax(getRxFrequency());
    offset = 0;
}