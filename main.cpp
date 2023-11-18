#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
// #include "hardware/timer.h"

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


    auto i2cInput = I2Cinput::getInstance();

    TrxStateVfo vfoA(VFO_A_INIT);
    TrxStateVfo vfoB(VFO_B_INIT);


    setTxAllowed(false);
    TrxState *currentState = isPressed("ab") ? &vfoB : &vfoA;

    Piezo::getInstance()->beepOK();


    // main loop
    while (true)
    {
        // sleep_ms(1);

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
            Piezo::getInstance()->beepOK();

        }
        if (wasPressed("stepDecrease") && isPressed("stepDecrease"))
        {
            currentState->stepDown();
            Piezo::getInstance()->beepOK();
        }        

        // read up and down buttons
        if (wasPressed("micUp") && isPressed("micUp")) // read event and state to avoid crosstalk effects
        {
            updown++;
            Piezo::getInstance()->beepOK();
        }
        if (wasPressed("micDown") && isPressed("micDown"))
        {
            updown--;
            Piezo::getInstance()->beepOK();
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

        

        // update display and ctcss encoder
        Display::getInstance()->update(*currentState);
        Ctcss::getInstance()->update(*currentState);

        // configure pll and tune drive unit
        ADF4351::getInstance()->write(currentState->getCurrentFrequency());
        writeDAC(dacValue(dacVoltage(currentState->getTxFrequency())));

        setTxAllowed(currentState->isTxAllowed());
    }
}