//Initial configuration of IO pins, IOC registers, and Timer 3 respectively
void IOinit(void);
void IOCconfig(void);
void T3config(void);

//Global variable to detect button press event
uint16_t PB_event;

//Macro definitions for LED and PB pins
#define LED0 LATBbits.LATB5
#define LED1 LATBbits.LATB6
#define LED2 LATBbits.LATB7
#define PB0 PORTBbits.RB3
#define PB1 PORTBbits.RB8
#define PB2 PORTBbits.RB10


//Interupt service routines
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _IOCInterrupt(void);