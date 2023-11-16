#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include "pico-ssd1306/ssd1306.h"

#include "settings.h"
#include "TrxStateVfo.h"
#include "Display.h"
#include "I2Cinput.h"
#include "GPIOinput.h"
#include "GPIOoutput.h"
#include "MCP4725.h"
#include "ADF4351.h"
#include "rotaryEncoder.pio.h"
#include "beep.pio.h"
#include "ctcss.pio.h"


int main()
{
    // setup I2C
    i2c_init(I2C_PORT, I2C_CLOCK);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // setup GPIO input
    for (auto [name, pin] : gpioInPins)
    {
        gpio_init(pin);
        gpio_pull_up(pin);
    }

    // setup GPIO output
    gpio_init(TX_FORBIDDEN_PIN);
    gpio_set_dir(TX_FORBIDDEN_PIN, true);


    // setup rotary encoder pio
    gpio_pull_up(ROTARY_CLOCK);
    gpio_pull_up(ROTARY_DATA);
    uint rotaryOffset = pio_add_program(ROTARY_PIO, &rotary_program);
    uint rotarySm = pio_claim_unused_sm(ROTARY_PIO, true);
    pio_sm_config rotaryConfig = rotary_program_get_default_config(rotaryOffset);
    pio_gpio_init(ROTARY_PIO, ROTARY_CLOCK);
    pio_gpio_init(ROTARY_PIO, ROTARY_DATA);
    sm_config_set_in_pins(&rotaryConfig, ROTARY_CLOCK);
    sm_config_set_jmp_pin(&rotaryConfig, ROTARY_DATA);
    sm_config_set_fifo_join(&rotaryConfig, PIO_FIFO_JOIN_RX);
    sm_config_set_in_shift(&rotaryConfig, false, false, 0);
    pio_sm_init(ROTARY_PIO, rotarySm, rotaryOffset, &rotaryConfig);
    pio_sm_set_enabled(ROTARY_PIO, rotarySm, true); 


    // setup beep pio
    uint beepOffset = pio_add_program(BEEP_PIO, &beep_program);
    uint beepSm = pio_claim_unused_sm(BEEP_PIO, true);
    pio_sm_config beepConfig = beep_program_get_default_config(beepOffset);
    sm_config_set_set_pins(&beepConfig, BEEP_PIN, 1);
    pio_gpio_init(BEEP_PIO, BEEP_PIN);
    pio_sm_init(BEEP_PIO, beepSm, beepOffset, &beepConfig);
    pio_sm_set_enabled(BEEP_PIO, beepSm, false); 
    
    
    // setup ctcss pio
    uint ctcssOffset = pio_add_program(CTCSS_PIO, &ctcss_program);
    uint ctcssSm = pio_claim_unused_sm(CTCSS_PIO, true);
    pio_sm_config ctcssConfig = ctcss_program_get_default_config(ctcssOffset);
    sm_config_set_set_pins(&ctcssConfig, CTCSS_PIN, 1);
    pio_gpio_init(CTCSS_PIO, CTCSS_PIN);
    pio_sm_init(CTCSS_PIO, ctcssSm, ctcssOffset, &ctcssConfig);
    pio_sm_set_enabled(CTCSS_PIO, ctcssSm, false);


    auto i2cInput = I2Cinput::getInstance();

    TrxStateVfo vfoA(VFO_A_INIT);
    TrxStateVfo vfoB(VFO_B_INIT);


    setTxAllowed(false);
    TrxState *currentState = isPressed("ab") ? &vfoB : &vfoA;

    beepOK(&beepConfig, beepSm);

    /*
    const double fCtcss = 151.4;
    const uint cycles = 180;
    const auto sysClock = clock_get_hz(clk_sys);
    const double clkDiv = sysClock / (fCtcss * cycles);
    pio_sm_set_clkdiv(CTCSS_PIO, ctcssSm, clkDiv);
    pio_sm_set_enabled(CTCSS_PIO, ctcssSm, true);
    */

    // main loop
    while (true)
    {
        sleep_ms(1);

        // read I²C input
        i2cInput->update(); // must be called in the main loop

        // read rotary encoder
        int updown = 0;
        while (!pio_sm_is_rx_fifo_empty(ROTARY_PIO, rotarySm))
        {
            updown += (int32_t) pio_sm_get(ROTARY_PIO, rotarySm);
        }

        // read step buttons
        if (wasPressed("stepIncrease") && isPressed("stepIncrease")) // read event and state to avoid crosstalk effects
        {
            currentState->stepUp();
            beepOK(&beepConfig, beepSm);

        }
        if (wasPressed("stepDecrease") && isPressed("stepDecrease"))
        {
            currentState->stepDown();
            beepOK(&beepConfig, beepSm);
        }        

        // read up and down buttons
        if (wasPressed("micUp") && isPressed("micUp")) // read event and state to avoid crosstalk effects
        {
            updown++;
            beepOK(&beepConfig, beepSm);
        }
        if (wasPressed("micDown") && isPressed("micDown"))
        {
            updown--;
            beepOK(&beepConfig, beepSm);
        }
        
        if (!isPressed("ptt"))
        {
            setTxAllowed(false);

            // read vfo switch
            currentState = isPressed("ab") ? &vfoB : &vfoA;  
            currentState->up(updown);
        }
        else // PTT pressed
        // The VFO wheel and the UP/DOWN buttons should work always in SSB and CW.
        {
            // TODO
        }

        

        // update display
        Display::getInstance()->update(*currentState);

        // configure pll and tune drive unit
        ADF4351::getInstance()->write(currentState->getCurrentFrequency());
        writeDAC(dacValue(dacVoltage(currentState->getTxFrequency())));

        setTxAllowed(currentState->isTxAllowed());
    }
}