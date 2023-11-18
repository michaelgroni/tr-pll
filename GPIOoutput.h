#pragma once

#include "hardware/pio.h"

void setTxAllowed(bool allowed);

void beepOK(pio_sm_config *c, uint sm);
void beepWriteOK(pio_sm_config *c, uint sm);
void beepError(pio_sm_config *c, uint sm);

class Piezo
{
public:
    static Piezo* getInstance();
    void beepOK();
    void beepWriteOK();
    void beepError();
private:
    Piezo();
    uint beepSm; // state machine
};

class Ctcss
{
public:
    static Ctcss* getInstance();
    void on(const double frequency) const;
    void off() const;
private:
    Ctcss();
    uint ctcssSm; // state machin
};