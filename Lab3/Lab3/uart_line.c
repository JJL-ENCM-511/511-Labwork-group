#include "uart.h"
#include <xc.h>
#include "config.h"

char BlinkSettingUart(void)
{
    Disp2String("Prog Mode: Blink setting = ");

    char last_char;
    XmitUART2('X',1);
    // wait for enter key
    while (1) {
        led0_blink(1);
        if (RXFlag == 1) {
            
            if (received_char == 0x0D) //insert pb1 check condition 
            {
                RXFlag = 0;
                return last_char;
            }
            
            // only store alphanumeric characters
            if (received_char >= 48 && received_char <= 50) // ignore char inputs that are not 0, 1 or 2
            {
                XmitUART2(0x08,1); // send backspace
                last_char = received_char;
                XmitUART2(received_char,1); // loop back display
            }
           
            U2STAbits.OERR = 0;
            RXFlag = 0;
        }
        
        // if (CNflag == 1) { // this allows breaking out of the busy wait if CN interrupts are enabled...
        //     add logic here
        // }
    }
}
