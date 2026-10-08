/*
 * File:   main.c
 * Author: Joshua Prud'Homme, Luke Zenha, Johann Yap
 *
 * Created FOR ENCM 511
 * PLEASE ADD DATE CREATED HERE: 2025-XX-XX
 */

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->Internal Fast RC (FRC)
#pragma config PLLMODE = PLL96DIV2    //PLL Mode Selection->96 MHz PLL. Oscillator input is divided by 2 (8 MHz input)
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFCN = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config SOSCSEL = OFF    //SOSC Power Selection Configuration bits->Digital (SCLKI) mode
#pragma config PLLSS = PLL_FRC    //PLL Secondary Selection Configuration bit->PLL is fed by the on-chip Fast RC (FRC) oscillator
#pragma config IOL1WAY = ON    //Peripheral pin select configuration bit->Allow only one reconfiguration
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config FWPSA = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config FWDTEN = ON_SWDTEN    //Watchdog Timer Enable bits->WDT Enabled/Disabled (controlled using SWDTEN bit)
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config WDTCMX = WDTCLK    //WDT MUX Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown Out Enable bit->Brown Out Enable Bit
#pragma config LPCFG = OFF    //Low power regulator control->No Retention Sleep
#pragma config DNVPEN = ENABLE    //Downside Voltage Protection Enable bit->Downside protection enabled using ZPBOR when BOR is inactive

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FDEVOPT1
#pragma config ALTCMPI = DISABLE    //Alternate Comparator Input Enable bit->C1INC, C2INC, and C3INC are on their standard pin locations
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config SOSCHP = ON    //SOSC High Power Enable bit (valid only when SOSCSEL = 1->Enable SOSC high power mode (default)
#pragma config ALTI2C1 = ALTI2CEN    //Alternate I2C pin Location->SDA1 and SCL1 on RB9 and RB8


#include "xc.h"
#include "config.h"
#include "delay_ms.h"

#define IOC




typedef enum 
{
    STATE_LED_DEFAULT,
    STATE_LED_250MS,
    STATE_LED_500MS,
    STATE_LED_BLINK_RATE
} state_0;

state_0 state_led = STATE_LED_DEFAULT;

typedef enum 
{
    STATE_PB_DEFAULT,
    STATE_PB2_WAIT,
    STATE_HALVE_BLINK_RATE
} state_1;

state_1 state_br = STATE_PB_DEFAULT;

uint16_t LED1_blink_rate = 32; // 0.125s * 32 = 4s

void led0_blink(int s); // Blink LED based on blink counter and blink rate
void led1_blink(int s); // Blink LED based on blink counter and blink rate
void halve_br(uint16_t *br_ptr); // halves blinkrate
void change_led_state(void); // changes state based on what buttons pressed




int main(void) {

    IOinit();

    IOCconfig();

    T2config();

    T3config();

    PB0PB1_event = 0; // software flag to detect change in button press
    PB2_event = 0;

    

    while(1)
    {
        if (PB0PB1_event)
        {
            PB0PB1_event = 0;
            //delay_ms(20);
            change_led_state();
        }
        if (PB2_event)
        {
            PB2_event = 0;
            //delay_ms(20);
            if (!PB2){
                state_br = STATE_PB2_WAIT;
            }
        }

        switch(state_led)
        {
            case STATE_LED_DEFAULT:
                led0_blink(0);
                led1_blink(0);
                break;
            case STATE_LED_250MS:
                led0_blink(2);
                led1_blink(0);
                break;
            case STATE_LED_500MS:
                led0_blink(4);
                led1_blink(0);
                break;
            case STATE_LED_BLINK_RATE:
                led0_blink(0);
                led1_blink(LED1_blink_rate);
                break;
        }
        switch(state_br)
        {
            case STATE_PB_DEFAULT:
                break;
            case STATE_PB2_WAIT:
                delay_ms(200);
                if (PB2)
                    break;
            case STATE_HALVE_BLINK_RATE:
                halve_br(&LED1_blink_rate);
                state_br = STATE_PB_DEFAULT;
                break;
        }


        Idle();
    }
    
    return 0;
}



void change_led_state(void)
{
    if (!PB0 && (PB1))
    {
        state_led = STATE_LED_250MS;
    }
    else if (!PB0 && !PB1)
    {
        state_led = STATE_LED_500MS;
    }
    else if ((PB0) && !PB1)
    {
        state_led = STATE_LED_BLINK_RATE;
    }
    else
    {
        state_led = STATE_LED_DEFAULT;
    }
}


void led0_blink(int s) // Blink LED based on blink counter and blink rate
{
    if (s == 0)
    {
        LED0 = 0;
    } 
    else if (blink_counter % s == 0 && blink_counter != 0)
    {
        LED0 ^= 1;
    }
}
void led1_blink(int s) // Blink LED based on blink counter and blink rate
{
    if (s == 0)
    {
        LED1 = 0;
    } 
    else if (blink_counter % s == 0 && blink_counter != 0)
    {
        LED1 ^= 1;
    }
}

void halve_br(uint16_t *br_ptr){
    
    if (*br_ptr > 1)
    {
        *br_ptr = *br_ptr / 2;
    } else {
        *br_ptr = 32;
    }
}









/*

if (PB0PB1_event)
        {
            PB0PB1_event = 0;
            // need to add delay here
            change_led_state();
        }
        if (PB2_event)
        {
            PB2_event = 0;
            if (PB2){
                state_br = STATE_PB2_WAIT;
            }
        }
        switch(state_led)
        {
            case STATE_LED_DEFAULT:
                led0_toggle(0);
                led1_toggle(0);
                break;
            case STATE_LED_250MS:
                led0_blink(250);
                break;
            case STATE_LED_500MS:
                led0_blink(500);
                break;
            case STATE_LED_BLINK_RATE:
                led1_blink(pb2_blink_rate);
                break;
        }
        switch(state_br)
        {
            case STATE_PB_DEFAULT:
                break;
            case STATE_PB2_WAIT:
                delay_ms(200);
                if (PB2)
                    break;
            case STATE_HALVE_BLINK_RATE:
                halve_br(&pb2_blink_rate);
                state_br = STATE_PB_DEFAULT;
                break;
        }
*/