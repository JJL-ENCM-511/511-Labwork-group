#include "xc.h"
#include "config.h"
#include "blink.h"

void led0_blink(int s) // Blink LED based on blink counter and blink rate
{
    if (s == 0)
    {
        LED0 = 0;
    } 
    else if (s == -1){
        LED0 = 1;
    }
    else if (blink_counter % s == 0 && blink_counter != 0)
    {
        LED0 ^= 1;
    }
}