#include "TrxStateScanMin.h"

uint32_t TrxStateScanMin::getRxFrequency() const
{
    return scanMin() + offset;
}

void TrxStateScanMin::up(int n)
{
    offset += n*getStep();

    if (getRxFrequency() >= scanMax())
    {
        offset = scanMax() - scanMin() - getStep();
    }

}

