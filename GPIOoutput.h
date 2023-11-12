#pragma once

#include "hardware/pio.h"

void setTxAllowed(bool allowed);

void beepOK(pio_sm_config *c, uint sm);
void beepWriteOK(pio_sm_config *c, uint sm);
void beepError(pio_sm_config *c, uint sm);