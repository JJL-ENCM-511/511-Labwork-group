/* =============================================================================
 * lab4_starter.c
 * ENCM 511, Fall 2026  --  Assignment 4 (Driver Project 4)
 * Motor Position Control: PWM, Timers, and Interrupts
 *
 * Target: PIC24FJ256GA702, 28-pin DIP
 *
 * WIRING (see lab schematic)
 *
 *   ENCODER  (Part 2 in-lab only)
 *     Orange (5V)  -> +5V rail
 *     Brown  (GND) -> GND rail
 *     Blue   (Ch A)-> Pin 16 (RB7)   IOC source, 5V-tolerant
 *     Yellow (Ch B)-> Pin 17 (RB8)   direction,  5V-tolerant
 *
 *   L298N  (Part 2 in-lab only)
 *     IN3  -> Pin 21 (RB10)    IN4  -> Pin 22 (RB11)
 *     ENB  -> Pin 24 (RB13)    *** Remove ENB jumper before wiring ***
 *
 *   PRELAB LED (remove before Part 2)
 *     Pin 24 (RB13) -> 330 Ohm -> LED -> GND
 *
 * CLOCKING: FRC, FOSC = 8 MHz, FCY = 4 MHz, TCY = 250 ns
 * =============================================================================
 */

#include <xc.h>
#include <stdint.h>

#pragma config FNOSC=FRC, POSCMD=NONE, OSCIOFCN=ON, FCKSM=CSDCMD
#pragma config IOL1WAY=OFF, FWDTEN=OFF, JTAGEN=OFF

#define FCY            4000000UL
#define COUNTS_PER_REV 2048
#define KP_Q8          13        // actual gain = KP_Q8 / 256 
#define PWM_MAX        399
#define REFERENCE      256

/* encoder_count: updated by _IOCInterrupt on every rising edge of Ch A.
 * volatile -- can change at any time from an interrupt.
 * int32_t  -- signed so CW (+) and CCW (-) are both represented.
 * Reading this 32-bit variable requires special care -- see run_controller(). */
volatile int32_t encoder_count = 0;

// control_flag: set every 10 ms by _T2Interrupt, cleared by main.
volatile uint8_t control_flag  = 0;

/* Function prototypes */
void clock_init(void);
void gpio_init(void);
void encoder_init(void);
void timer_init(void);
void pwm_init(void);
void set_motor_pwm(int32_t control);
void run_controller(void);


/* clock_init: configure the device to run at FCY = 4 MHz.
 * Datasheet ref: Section 9.1, Register 9-2 (CLKDIV). */
void clock_init(void) {
    CLKDIVbits.RCDIV = ??;   // Change this to run at 4 MHz (CLK divides by 1)
}


/* gpio_init: configure all digital I/O pins used in this assignment.
 * Input Pins: RB7, RB8
 * Output Pins: RB10, RB11, RB13
 * Note: RB13 is configured in pwm_init() -- do not touch it here. */
void gpio_init(void) {
    
    ANSELA = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */
    ANSELB = 0x0000; /* keep this line as it sets I/O pins that can also be analog to be digital */

    TRISBbits.TRISB10 = 0; //
    TRISBbits.TRISB11 = 0; //set RB10 and RB11 as output 

    TRISBbits.TRISB7 = 1; //
    TRISBbits.TRISB8 = 1; //set RB7 and RB8 as input

    IOCPUBbits.CNPUB7 = 1; //
    IOCPUBbits.CNPUB8 = 1; //activate pullup for RB7 and RB8

}


/* encoder_init: configure the IOC interrupt on channel A (RB7).
 * The ISR reads the live RB7 state to keep only rising edges.
 * Datasheet ref: Section 11.3. */
void encoder_init(void) {
    PADCONbits.IOCON   = 0;     // disable IOC while configuring -- DO NOT CHANGE 

    IOCPBbits.IOCPB7   = 1;     // enable rising edge on RB7  
    IOCNBbits.IOCNB7   = 0;     // disable falling edge on RB7  

    IOCFB              = 0;     // DO NOT CHANGE 
    IOCSTATbits.IOCPBF = 0;     // DO NOT CHANGE 
    PADCONbits.IOCON   = 1;     // DO NOT CHANGE 
    IFS1bits.IOCIF     = 0;     // DO NOT CHANGE 

    IPC4bits.IOCIP     = 3;     // IOC priority  
    IEC1bits.IOCIE     = 1;     // IOC interrupt enable 
}


/* timer_init: configure Timer2 to interrupt at 100 Hz (every 10 ms).
 * All lines are present -- set each value correctly.
 * Datasheet ref: Section 13, Register 13-1 (T2CON).
 * MUST be called last in main() -- see main() comment. */
void timer_init(void) {
    T2CON = 0;                  // DO NOT CHANGE 
    TMR2  = 0;                  // DO NOT CHANGE 

    PR2               = ??;      // period register    
    T2CONbits.TCKPS   = ??;   // prescaler select   
    T2CONbits.TCS     = ??;      // clock source       
    T2CONbits.T32     = ??;      // timer width        

    IFS0bits.T2IF     = ??;      // DO NOT CHANGE      
    IPC1bits.T2IP     = ??;      // Timer2 priority    
    IEC0bits.T2IE     = ??;      // interrupt enable   
    T2CONbits.TON     = ??;      // start timer        
}


/* pwm_init: configure OC1 in edge-aligned PWM mode on RB13 (ENB).
 * Implement from scratch.
 *   - RB13 has an analog function that must be disabled before digital use.
 *   - OC1 must be routed to RB13 via the PPS system.
 *   - Set OC1RS for a 10 kHz PWM frequency.
 *   - Set the OCM bits to enable edge-aligned PWM mode.
 * Datasheet ref: Sections 11.5 (PPS) and 15.3 (Output Compare). */
void pwm_init(void) {
    // your code here 
}


/* set_motor_pwm: apply a signed controller output to the L298N.
 *   control > 0  ->  CW  (IN3=1, IN4=0)
 *   control < 0  ->  CCW (IN3=0, IN4=1)
 *   control = 0  ->  stopped
 * Clear both direction pins before setting the new direction.
 * The duty cycle written to OC1R must be non-negative and clamped to PWM_MAX. */
void set_motor_pwm(int32_t control) {
    int32_t magnitude;
    // your code here 
}


/* run_controller: compute the P-control output and drive the motor.
 * NO floating-point. Use Q8 fixed-point: u = (KP_Q8 * error) >> 8
 *
 * WHY __builtin_disi IS NEEDED:
 *   encoder_count is 32-bit on a 16-bit CPU -- reading it takes two load
 *   instructions. If _IOCInterrupt fires between them, the two halves of
 *   'position' come from different ISR updates (corrupted read).
 *   disi(0x3FFF) disables interrupts for those two instructions only.
 *   Clearing IEC1bits.IOCIE instead risks silently dropping an encoder
 *   edge -- a position error the controller can never recover from. */
void run_controller(void) {
    int32_t position;
    int32_t error;
    int32_t u;

    __builtin_disi(0x3FFF);         // DO NOT CHANGE 
    position = encoder_count;       // DO NOT CHANGE
    __builtin_disi(0x0000);         // DO NOT CHANGE

    // write your control algorithm here
}


/* _IOCInterrupt: fires on every edge of channel A (RB7).
 * Implement from scratch:
 *   - Confirm the source is RB7, then keep rising edges only.
 *   - Increment or decrement encoder_count based on channel B.
 *   - Clear flags in this exact order: IOCFB7 -> IOCPBF -> IOCIF.
 * No function calls, UART, or delays inside this ISR. */
void __attribute__((interrupt, no_auto_psv)) _IOCInterrupt(void) {
    if (IOCFBbits.IOCFB7)
    {
        if (PORTBbits.RB8) encoder_count ++;    
        else encoder_count --;
        IOCFBbits.IOCFB7 = 0;
    }

    IOCSTATbits.IOCPBF = 0;
    IFS1bits.IOCIF = 0;
}


/* _T2Interrupt: DO NOT MODIFY. */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    control_flag  = 1;
    IFS0bits.T2IF = 0;
}

/* main: initialise peripherals then run the control super-loop.
 *
 * Init order:
 *   clock_init() -> gpio_init() -> pwm_init() -> encoder_init() -> timer_init()
 *
 * timer_init() must be last: starting Timer2 before other peripherals are
 * configured means _T2Interrupt can fire before PWM and the encoder are ready. */
int main(void) {
    IFS0 = 0; IFS1 = 0; IFS2 = 0; IFS3 = 0; IFS4 = 0;

    // your initialisation calls here 

    while (1) {
        // your code here
    }

    return 0;
}
