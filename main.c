/*
 * File:   main.c
 * Author: Lucas Zenha, Johan Yap, Joshua Prud'Homme
 *
 * Created FOR ENCM 511
 * 2026/09/15
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
#define PB0 PORTBbits.RB8
#define PB1 PORTAbits.RA4

void ledOn()
{
    LED0 = 1;
}

void ledBlink(int delay, int in_delay)
{
    LED0 = 0;
    for (int i = 0; i < delay ; i++)
    {
        for (int j = 0; j < in_delay; j++){}
    }
    LED0 = 1;
    for (int i = 0; i < delay ; i++)
    {
        for (int j = 0; j < in_delay; j++){}
    }
    
}

void ledButtonOn()
{
    if (PB0 == 1)
    {
        LED0 = 0;
    }
    else
    {
        LED0 = 1;
    }
}

int main(void) {
    
    TRISBbits.TRISB5 = 0; //set RB5 as output
    TRISBbits.TRISB6 = 0; //set RB6 as output
    TRISBbits.TRISB7 = 0; //set RB7 as output
    TRISBbits.TRISB8 = 1; //set RB8 as input
    TRISAbits.TRISA4 = 1; //set RA4 as input
    IOCPUBbits.CNPUB8 = 1; //activate pullup for RB8
    IOCPUAbits.CNPUA4 = 1; //activate pullup for RA4
    
    int loopCount = 0;


    while(1)
    {
        //1 second
        //ledBlink(10000, 50);

        //0.25 second
        //ledBlink(2500, 50);

        //ledButtonOn();
        

        // Continuously check the state of both psuh buttons for 0.25 s to control LED2
        for (int i = 0; i < 2500 ; i++){
            for (int j = 0; j < 50; j++){
                if(PB0 == 0 && PB1 == 0){
                    LED2 = 1;
                }
                else{
                    LED2 = 0;
                }
            }
        }
        //Check status of PB0 for LED0
        
        
        if(PB0 == 0){
            LED0 ^= 1; //toggle LED0
        }
        else{
            LED0 = 0; // Disable LED0 if PB0 is not pressed
        }

        if(PB1 == 0){
            if(loopCount > 3){
                LED1 ^= 1; //toggle LED1
            }
        }
        else{
            LED1 = 0; // Disable LED1 if PB1 is not pressed
        }

        if(loopCount > 3){
            loopCount = 0; // Reset loopCount after 4 iterations
        }
        else{
            loopCount++; // Increment loopCount
        } 

        
    }
    
    return 0;
}
