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


#define LED0 LATBbits.LATB5
#define LED1 LATBbits.LATB6
#define LED2 LATBbits.LATB7
#define PB0 PORTBbits.RB3
#define PB1 PORTAbits.RB8
#define PB2 PORTAbits.RB10

#define IOC

uint16_t PB_event;
uint16_t toggle = 0;

typedef enum 
{
    STATE_DEFAULT,
    STATE_LED_250MS,
    STATE_LED_500MS,
    STATE_LED_BLINK_RATE,
    STATE_PB2_WAIT,
    STATE_HALVE_BLINK_RATE
} state_t

state_t state = STATE_DEFAULT;

void IOinit(void);
void blink(float interval); // blink in ms
void led_switch(int s);
void halve_br(float *br_ptr);
void change_state(void);

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void);

int main(void) {
    ANSELA = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    ANSELB = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */

    //T3CON config
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T3CONbits.TCKPS = 3; // set prescaler to 1:8
    T3CONbits.TCS = 0; // use internal clock
    T3CONbits.TSIDL = 0; //operate in idle mode
    IPC2bits.T3IP = 2; //7 is highest and 1 is lowest pri.
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //enable timer interrupt
    PR3 = 7812; // set the count value for 0.5 s (or 500 ms)
    TMR3 = 0;
    T3CONbits.TON = 1;

    IOinit();

    PADCONbits.IOCON = 1;

    IOCNBbits.IOCNB10 = 1;
    IOCPBbits.IOCPB10 = 1;

    IOCNBbits.IOCNB8 = 1;
    IOCPBbits.IOCPB8 = 1;

    IOCNBbits.IOCNA4 = 1;
    IOCPBbits.IOCPA4 = 1;

    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;

    PB_event = 0; // software flag to detect change in button press

    float pb2_blink_rate = 4; // seconds

    while(1)
    {
        if (PB_event)
        {
            PB_event = 0;
            // need to add delay here
            change_state();
        }

        switch(state)
        {
            case STATE_DEFAULT:
                led_switch(0);
                break;
            case STATE_LED_250MS:
                blink(250)
                break;
            case STATE_LED_500MS:
                blink(500)
                break;
            case STATE_LED_BLINK_RATE:
                blink(pb2_blink_rate)
                break;
            case STATE_PB2_WAIT:
                delay()
                if (PB2)
                    break;
            case STATE_HALVE_BLINK_RATE:
                halve_br(&pb2_blink_rate);
                state = STATE_DEFAULT;
                break;
        }
    }
    
    return 0;
}

void IOinit(void)
{
    TRISBbits.TRISB5 = 0; //set RB5 as output (LED0)
    TRISBbits.TRISB6 = 0; //set RB6 as output (LED1)
    TRISBbits.TRISB7 = 0; //set RB7 as output (LED2)

    TRISBbits.TRISB8 = 1; //set RB8 as input (PB2)
    TRISAbits.TRISB3 = 1; //set RA4 as input (PB0)
    TRISAbits.TRISB10 = 1; //set RB10 as input (PB3)

    IOCPUBbits.CNPUB8 = 1; //activate pullup for RB8
    IOCPUAbits.CNPUB3 = 1; //activate pullup for RB3
    IOCPUAbits.CNPUB10 = 1; //activate pullup for RB10
}

void change_state(void)
{
    if (PB0 && (!PB1))
    {
        state = STATE_LED_250MS;
    }
    else if (PB0 && PB1)
    {
        state = STATE_LED_500MS;
    }
    else if ((!PB0) && PB1)
    {
        state = STATE_LED_BLINK_RATE;
    }
    else if (PB2)
    {
        state = STATE_PB2_WAIT;
    }
    else
    {
        state = STATE_DEFAULT;
    }
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T3IF = 0;
    LED0 ^= 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {
    PB_event = 1;

    // clear flags
    IOCFBbits.IOCFB3 = 0
    IOCFBbits.IOCFB8 = 0
    IOCFBbits.IOCFB10 = 0
    IOCSTATbits.IOCPBF = 0
    IFS1bits.IOCIF = 0;
}