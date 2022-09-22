#include "mbed.h"
#include "sml.hpp" // [Boost::ext].SML

InterruptIn g_ButtonOne(SW2);
DigitalOut  g_UserLED(LED1);

void riseHandler()
{

}

int main()
{
    g_ButtonOne.rise(&riseHandler); // attach the address of the flip function to the rising edge
    
    while (1) 
    {          // wait around, interrupts will interrupt this!
        led = !led;
        ThisThread::sleep_for(250);
    }
}
