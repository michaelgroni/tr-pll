#include "GPIOinput.h"

#include "settings.h"

#include "hardware/timer.h"


void setTxAllowed(bool allowed)
{
    if (allowed)
    {
        gpio_put(TX_FORBIDDEN_PIN, 0);
    }
    else
    {
        gpio_put(TX_FORBIDDEN_PIN, 1);
    }
}


void beepOK(pio_sm_config *c, uint sm)
{
    sm_config_set_clkdiv(c, 12500);
    pio_sm_set_enabled(BEEP_PIO, sm, true);
    sleep_ms(75);
    pio_sm_set_enabled(BEEP_PIO, sm, false);
}


void beepWriteOK(pio_sm_config *c, uint sm)
{
    sm_config_set_clkdiv(c, 15625);
    pio_sm_set_enabled(BEEP_PIO, sm, true);
    sleep_ms(250);
    pio_sm_set_enabled(BEEP_PIO, sm, false);
}

    
void beepError(pio_sm_config *c, uint sm)
{
    sm_config_set_clkdiv(c, 62500);
    for (int i=0; i<4; i++)
    {
        pio_sm_set_enabled(BEEP_PIO, sm, true);
        sleep_ms(50);
        pio_sm_set_enabled(BEEP_PIO, sm, false);
        sleep_ms(50);
    }
    
}
    