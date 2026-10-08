#include "delay_ms.h"
#include "xc.h"


void delay_ms(uint16_t ms)
{
    // Fcy is FOSC/2 = 16MHz Tcy = 1/Fcy = 62.5ns
    // Overflow time = # of Count * Tcy * prescaler
    // Time each tick = prescaler * Tcy = 8 * 62.5ns = 500ns
    int flag_triggered = 0;
    PR2 = ms * 2000000; // set the count value for variable value in ms (each tick is 500ns so to get 1ms * delay_ms converting each tick is 2,000,000)
    TMR2 = 0;       // initialize the actualy counting register
    T2CONbits.TON = 1;  //enable timer 2

    while(IFS0bits.T2IF == 0) {
        if(flag_triggered == 1)
        {
            flag_triggered = 0;
            break;
        }
    }; // wait for the timer to overflow
    T2CONbits.TON = 0; //disable timer 2
}
    
