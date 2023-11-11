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
#include "ADF4351.h"


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
  

    auto i2cInput = I2Cinput::getInstance();

    TrxStateVfo vfoA(VFO_A_INIT);
    TrxStateVfo vfoB(VFO_B_INIT);


    // main loop
    setTxAllowed(false);
    TrxState *currentState = isPressed("ab") ? &vfoB : &vfoA;

    while (true)
    {
        // read I²C input
        i2cInput->update(); // must be called in the main loop
        
        if (!isPressed("ptt"))
        {
            setTxAllowed(false);

            // read vfo switch
            currentState = isPressed("ab") ? &vfoB : &vfoA;   
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