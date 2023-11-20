#pragma once

#include "TrxStateVfo.h"

#include "pico/stdlib.h"

class Scanner
{
private:
    bool up = true;
    bool on = false;
    bool wasOpen = false;

public:
    void update(TrxStateVfo* trxStateVfo);
    void setUp(bool up);
    void setOn(bool on);
    bool isOn();
};