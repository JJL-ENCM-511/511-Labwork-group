#include "delay_ms.h"
#include "xc.h"


void delay_ms(uint16_t delay_ms)
{
    // Fcy is FOSC/2 = 16MHz Tcy = 1/Fcy = 62.5ns
    // Overflow time = # of Count * Tcy * prescaler
    // Time each tick = prescaler * Tcy = 1 * 62.5ns = 62.5ns
    PR2 = delay_ms * 16000000; // set the count value for variable value in ms (each tick is 62.5ns so to get 1ms * delay_ms converting each tick is 16,000,000)
    TMR2 = 0;       // initialize the actualy counting register
    T2CONbits.TON = 1;  //enable timer 2

    if(IFS0bits.T2IF == 1) {    // this flag should only go up when TMR2 = PR2 (right?)
        _T2Interrupt();    // trigger interrupt event which clears interrupt flag?
        T2CONbits.TON = 0; //disable timer 2
    }
}