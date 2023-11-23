#include "TrxStateScanMin.h"

#include "settings.h"

uint32_t TrxStateScanMin::getRxFrequency() const
{
    return scanMin() + offset;
}

void TrxStateScanMin::up(int n)
{
    while ( getRxFrequency() % getStep() != 0) // step could have been changed
    {
        offset--;
    }

    offset += n*getStep();

    if (getRxFrequency() > F_MAX)
    {
        offset = F_MAX - scanMin();
    }
    else if (getRxFrequency() < F_MIN)
    {
        offset = F_MIN - scanMin();
    }
}

void TrxStateScanMin::save()
{
    saveScanMin(getRxFrequency());
    offset = 0;
}