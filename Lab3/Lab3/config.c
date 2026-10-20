#include "xc.h"
#include "config.h"

uint16_t PB0_event;
uint16_t PB1_event;
uint16_t PB2_event;
uint16_t mode_switch_event;
uint16_t dual_PB_event;
uint16_t PB_event;
uint16_t blink_counter = 0;
uint16_t flag_triggered = 0;

void IOinit(void)
{
    ANSELA = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    ANSELB = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */

    TRISBbits.TRISB5 = 0; //set RB5 as output (LED0)
    TRISBbits.TRISB6 = 0; //set RB6 as output (LED1)
    TRISBbits.TRISB7 = 0; //set RB7 as output (LED2)

    TRISBbits.TRISB3 = 1; //set RB3 as input (PB0)
    TRISBbits.TRISB8 = 1; //set RB8 as input (PB1)
    TRISBbits.TRISB12 = 1; //set RB12 as input (PB2)

    IOCPUBbits.CNPUB8 = 1; //activate pullup for RB8
    IOCPUBbits.CNPUB3 = 1; //activate pullup for RB3
    IOCPUBbits.CNPUB12 = 1; //activate pullup for RB12
}


void IOCconfig(void)
{
    PADCONbits.IOCON = 1;

    IOCNBbits.IOCNB12 = 1;
    IOCPBbits.IOCPB12 = 0;  // Changed to 0 so that logic only works on a falling edge (release of PB)

    IOCNBbits.IOCNB8 = 1;
    IOCPBbits.IOCPB8 = 0;   // Changed to 0 so that logic only works on a falling edge (release of PB)

    IOCNBbits.IOCNB3 = 1;
    IOCPBbits.IOCPB3 = 0;   // Changed to 0 so that logic only works on a falling edge (release of PB)

    IOCSTATbits.IOCPBF = 0;
    
    IFS1bits.IOCIF = 0;
    IPC4bits.IOCIP = 3;
    IEC1bits.IOCIE = 1;
}

void T2config(void)
{
    //T2CON config
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer 
    T2CONbits.TCKPS = 1; // set prescaler to 1:8
    T2CONbits.TCS = 0; // use internal clock (Fosc/2 givs the clk for the timer when Tcs is set to 0)
    T2CONbits.TSIDL = 0; // operate in idle mode
    IPC2 = 2; // 7 is highest and 1 is lowest priority
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1; //enable timer 2 interrupt
}

void T3config(void)
{
    //T3CON config
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T3CONbits.TCKPS = 3; // set prescaler to 1:256  {Might need to change to 1:1}
    T3CONbits.TCS = 0; // use internal clock    {Might need to change to FRC}
    T3CONbits.TSIDL = 0; //operate in idle mode
    IPC2bits.T3IP = 2; //7 is highest and 1 is lowest pri.
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //enable timer interrupt
    PR3 = 1953; // set the count value for 0.125s 
    TMR3 = 0;
    T3CONbits.TON = 1;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){    //Interrupt only triggers during delay_ms function
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0;
    flag_triggered = 1;

}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void){
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T3IF = 0;
    if (blink_counter <= 32)
    {
        blink_counter++;
    } else {
        blink_counter = 0;
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void) {

    PB_event = 1; // Flag to indicate that a PB event has occurred

    if (IOCFBbits.IOCFB3)   // PB 0 pressed
    {
        PB0_event = 1;
        IOCFBbits.IOCFB3 = 0;
    }

    if (IOCFBbits.IOCFB8)   //PB 1 pressed
    {
        PB1_event = 1;
        IOCFBbits.IOCFB8 = 0;
    }

    if (IOCFBbits.IOCFB12)  //PB 2 pressed
    {
        PB2_event = 1;
        IOCFBbits.IOCFB12 = 0;
    }

    // 2 PBs pressed at same time 
    // **THIS CHECK IS DEPENDANT ON ORDER PB CONDITIONS MUST BE CHECKED BEFORE THIS CONDITION TO PROPERLY FUNCTION**
    if (PB0_event && PB1_event ||     // PB0, PB1 both pressed
        PB0_event && PB2_event ||     // PB0, PB2 both pressed
        PB1_event && PB2_event ) {  // PB1, PB2 both pressed
    
        dual_PB_event = 1;      //Flags triggered are reset in earlier checking statements
    }

    // clear IOC interrupt flags
    IOCSTATbits.IOCPBF = 0;
    IFS1bits.IOCIF = 0;
}
