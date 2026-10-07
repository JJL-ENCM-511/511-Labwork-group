#include "xc.h"
#include "config.h"


void IOinit(void)
{
    ANSELA = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    ANSELB = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */

    TRISBbits.TRISB5 = 0; //set RB5 as output (LED0)
    TRISBbits.TRISB6 = 0; //set RB6 as output (LED1)
    TRISBbits.TRISB7 = 0; //set RB7 as output (LED2)

    TRISBbits.TRISB8 = 1; //set RB8 as input (PB2)
    TRISBbits.TRISB3 = 1; //set RB3 as input (PB0)
    TRISBbits.TRISB10 = 1; //set RB10 as input (PB3)

    IOCPUBbits.CNPUB8 = 1; //activate pullup for RB8
    IOCPUBbits.CNPUB3 = 1; //activate pullup for RB3
    IOCPUBbits.CNPUB10 = 1; //activate pullup for RB10
}


void IOCconfig(void)
{
    PADCONbits.IOCON = 1;

    IOCNBbits.IOCNB10 = 1;
    IOCPBbits.IOCPB10 = 1;

    IOCNBbits.IOCNB8 = 1;
    IOCPBbits.IOCPB8 = 1;

    IOCNAbits.IOCNA4 = 1;
    IOCPAbits.IOCPA4 = 1;

    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;
}

void T2config(void)
{
    //T2CON config
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer 
    T2CONbits.TCKPS = 0; // set prescaler to 1:1
    T2CONbits.TCS = 0; // use internal clock (Fosc/2 givs the clk for the timer when Tcs is set to 0)
    T2CONbits.TSIDL = 0; // operate in idle mode
    IPC2bits.T2IP = 2; // 7 is highest and 1 is lowest priority
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1; //enable timer 2 interrupt
}

void T3config(void)
{
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
    IOCFBbits.IOCFB3 = 0;
    IOCFBbits.IOCFB8 = 0;
    IOCFBbits.IOCFB10 = 0;
    IOCSTATbits.IOCPBF = 0;
    IFS1bits.IOCIF = 0;
}