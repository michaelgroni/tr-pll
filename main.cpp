#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"

#include "pico-ssd1306/ssd1306.h"

#include "settings.h"
#include "TrxStateVfo.h"
#include "Display.h"
#include "I2Cinput.h"
#include "GPIOinput.h"
#include "MCP4725.h"


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
    setTxAllowed(false);

    auto i2cInput = I2Cinput::getInstance();

    TrxStateVfo vfoA(VFO_A_INIT);
    TrxStateVfo vfoB(VFO_B_INIT);

    // main loop
    while (true)
    {
        // read I²C input
        i2cInput->update(); // must be called in the main loop
        
        if (!isPressed("ptt"))
        {
            // read vfo switch
            TrxState *currentState = isPressed("ab") ? &vfoB : &vfoA;

            // tune drive unit
            setTxAllowed(false);
            writeDAC(dacValue(dacVoltage(currentState->getTxFrequency())));
            setTxAllowed(currentState->isTxAllowed());
            
            // update display
            Display::getInstance()->update(*currentState);
        }
        else // PTT pressed
        // A change of modulation type must be processed even if the PTT is pressed.
        // The VFO wheel and the UP/DOWN buttons should work always in SSB and CW.
        {
            // TODO
        }
    }
}