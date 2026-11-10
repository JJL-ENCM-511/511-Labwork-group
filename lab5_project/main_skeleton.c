/*
 * ENCM 511 - Assignment 5 : Closed-loop boost PI controller (STUDENT SKELETON)
 *
 * You implement the functions marked TODO. The handout explains the architecture and
 * tells you WHAT each function must do and WHERE in the datasheet to look - it does not
 * give you the code.
 *
 * Provided for you (do NOT rewrite):
 *   - the #pragma config lines, the globals, and the #define constants, except that
 *     you add INTEGRATOR_MAX yourself and change K_P and K_I when you tune the loop in
 *     Part 2 (and MAX_OUTPUT_VOLTS only if you use different divider resistors)
 *   - main() : the port setup and the flag-driven control loop, apart from the two
 *              control-term lines marked TODO inside the loop
 *   - get_ADC_voltage_Q8() : turns the 12-bit code your ADC_read() returns into a Q8 voltage
 *   - pwm_init() : the PWM (Output Compare) initialization
 *
 * You write:
 *   Part 1 - timer_init, the Timer2 ISR, ADC_init, ADC_read, and set_pwm
 *   Part 2 - the INTEGRATOR_MAX #define, update_accumulator, the two control terms inside
 *            main()'s loop, and set_control_signals
 *
 * Every function you write has its instructions in the comment block directly above it.
 *
 * A working UART library (uart.c / uart.h) is available if you want to print values
 * while debugging. It is optional and is not part of the assignment.
 */

#define FCY 4000000UL // 4 MHz instruction clock (Fcy)

#include <xc.h>
#include <libpic30.h>
#include <stdint.h>
// #include "uart.h"   // optional: provided library for your own debug prints

// Where Fcy = 4 MHz comes from: FNOSC = FRC selects the 8 MHz internal Fast RC oscillator
// with no PLL, and the PIC24F always divides Fosc by two to produce the instruction cycle
// clock, so Fcy = 8 MHz / 2 = 4 MHz. This is fixed by the configuration bits below - no
// run-time register write sets it. TIMER_PERIOD and the PWM period are both derived from
// that 4 MHz. See datasheet Section 9.2 (Oscillator Configuration).
#pragma config FNOSC=FRC, POSCMD=NONE, OSCIOFCN=ON, FCKSM=CSDCMD
#pragma config IOL1WAY=OFF, FWDTEN=OFF, JTAGEN=OFF

// Several values below are stored in fixed point: the real number is multiplied
// by 2^FRAC_BITS so the control loop can use fast integer math instead of floats.
// Example: 1.5 volts is stored as 1.5 * 2^8 = 384.
#define FRAC_BITS 8

#define ADC_BITS      12     // ADC resolution: 12-bit -> codes 0..4095
#define ADC_MAX_VOLTS 3.3    // pin voltage that produces a full-scale ADC reading (AVdd)

// A voltage divider scales the converter output down to the ADC range, so that
// MAX_OUTPUT_VOLTS at the output reads as ADC_MAX_VOLTS at the ADC pin.
#define TARGET_OUTPUT_VOLTS 12   // voltage we want the boost converter to produce
#define MAX_OUTPUT_VOLTS    25   // output voltage that maps to a full-scale reading

// Controller setpoint, in the same units the loop measures in:
//     (voltage at the ADC pin) * 2^FRAC_BITS
// Built entirely from constants, so it is computed at compile time (no float on the chip).
#define REFERENCE ((int32_t)(TARGET_OUTPUT_VOLTS * ADC_MAX_VOLTS / MAX_OUTPUT_VOLTS * (1L << FRAC_BITS)))

// Timer2 fires SAMPLE_RATE_HZ times per second. The rate is DEFINED as a power of
// two, 2^SAMPLE_RATE_BITS, so the integral term can divide by it with a single right
// shift of SAMPLE_RATE_BITS instead of a divide. Defining it this way means the two
// constants can never disagree: to change the rate, change SAMPLE_RATE_BITS only.
#define SAMPLE_RATE_BITS 13                                  // sample rate = 2^13
#define SAMPLE_RATE_HZ   (1L << SAMPLE_RATE_BITS)            // = 8192 Hz
#define TIMER_PERIOD     ((4000000 / SAMPLE_RATE_HZ) - 1)    // PR2 value at Fcy = 4 MHz

// PI controller gains
#define K_P 100   // proportional gain (you tune this in Part 2)
#define K_I 400   // integral gain (you tune this in Part 2)

// PWM related variables
#define PWM_PERIOD   399   // OC1RS: PWM period -> 4 MHz / (399 + 1) = 10 kHz
#define PWM_DUTY_MAX 320   // clamp duty cycle to 80% (320 / 400)
#define PWM_DUTY_MIN 0

/* ---------- Shared globals ---------- */

// Set by the Timer2 ISR, cleared by main(): "a new control tick is due."
// 8-bit -> atomic; volatile because the ISR changes it behind main()'s back.
volatile uint8_t timer_flag = 0;

// Running total for the integral term. Only main() touches it, so it is single-context
// and needs no guard (see the software architecture section of the handout).
int32_t integral_accumulator = 0;


/* =============================== YOU IMPLEMENT (Part 1) =============================== */

/* timer_init()
 *
 * TODO: configure Timer2 in 16-bit mode, with its interrupt firing at SAMPLE_RATE_HZ, using
 *   TIMER_PERIOD as its period. Work through the Stage 1 checklist in the handout: clock
 *   source, prescaler, counter width, the interrupt flag / priority / enable, and starting
 *   the timer. See datasheet Section 13 (Timer2/3) and Section 8, Table 8-2 (Interrupt
 *   Vector Details), which lists the IFS/IEC/IPC bits for Timer2.
 */
void timer_init(void) {

}

/* ADC_init()
 *
 * TODO: configure the ADC to convert the boost feedback voltage on AN1/RA1 at 12-bit
 *   resolution. main() has already made AN1 analog, so setting this pin up as an analog
 *   input is not a part of your ADC setup. See datasheet Section 24 (A/D Converter),
 *   specifically the AD1CON1/AD1CON2/AD1CON3/AD1CHS registers: set input channel, voltage
 *   references, output format, the sample/convert sequence, the acquisition time, and the
 *   conversion clock. Then set operating mode to ON.
 *
 *   Names in code: the reference bits are AD1CON2bits.PVCFG (2 bits) and AD1CON2bits.NVCFG0.
 *   There is no NVCFG field. NVCFG0 = 1 would make RA1 itself the negative reference.
 */
void ADC_init(void) {

}

/* ADC_read()
 *
 * TODO: perform one sample-and-convert and return the 12-bit right-justified result
 *   (0..4095). How you detect that a conversion has finished, and how you get from
 *   ADC1BUF0 to a 0..4095 code, both follow from the choices you made in ADC_init().
 *
 *   Hint: clear DONE yourself just before you set SAMP. DONE can still be 1 from the
 *   previous conversion, and polling it without clearing it first can return the old result.
 */
uint16_t ADC_read(void) {
    return 0;   // replace with your reading
}

/* set_pwm()
 *
 * TODO: clamp duty to [PWM_DUTY_MIN, PWM_DUTY_MAX], then write it to the OC1 duty
 *   register so a runaway controller can never exceed the safe duty cycle.
 */
void set_pwm(int32_t duty) {

}

/* _T2Interrupt()
 *
 * TODO: keep this ISR minimal - tell main() that a tick is due, and acknowledge the
 *   interrupt in hardware. Nothing else (see the ISR rules in the handout).
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {

}


/* =============================== YOU IMPLEMENT (Part 2 - the controller) ============== */

/* update_accumulator()
 *
 * TODO: add error to integral_accumulator and clamp the result to [0, INTEGRATOR_MAX].
 *   The floor at 0 stops the accumulator going negative when the output overshoots. The
 *   ceiling is anti-windup, and it is also what keeps the integral arithmetic inside int32.
 *
 *   INTEGRATOR_MAX is not defined anywhere above - you add that #define yourself, and
 *   this function will not compile until you do. Start INTEGRATOR_MAX at 0: that pins the
 *   accumulator at zero and switches the integral term off entirely, so you can bring
 *   the proportional path up on its own first (see the Part 2 bring-up steps). Then
 *   raise it. There is no formula for the final value - too low and the integrator
 *   saturates before it has removed the error, too high and it keeps winding up past
 *   the point the output reached the reference, which you pay for in overshoot and a
 *   long, ringing settle. Expect to try several.
 *
 *   XC16 note: int is 16 bits. A constant like (384 << 13) is computed in 16-bit int and
 *   overflows to 0. Write (384L << 13) or cast to int32_t for anything above 32767.
 */
void update_accumulator(int32_t error) {

}

/* set_control_signals()
 *
 * main() has already applied a gain to each of these - they are the two halves of the
 * PI control signal u = Kp*e + Ki*integral(e):
 *     * proportional_action is the P half
 *     * integral_action     is the I half
 * Both of them arrive here in Q8: the error and the accumulator are both Q8, and
 * multiplying a Q8 value by a plain integer gain leaves the result in Q8.
 *
 * TODO: add the two actions together and shift the sum back out of Q8, then pass the
 *   result to set_pwm(). What reaches set_pwm() must be a plain duty-cycle count, not a
 *   Q8 value: hand a Q8 number to the duty register and you are out by a factor of 256.
 *   Work out the shift amount yourself (see the "fixed-point" section of the handout).
 */
void set_control_signals(int32_t proportional_action, int32_t integral_action) {

}


/* =============================== PROVIDED: Q8 conversion =============================== */

// Turns the 12-bit code your ADC_read() returns into the voltage at the ADC pin,
// expressed in Q8 (fixed-point: 8 integer bits, 8 fractional bits). At full scale that is
// 3.3 * 256 = 844, so the result needs all 16 bits - do not mask it down to 8.
//
// ADC_FULL_SCALE_Q8 is cast to an integer, which makes it a constant expression the
// compiler folds at build time. Left as a double it would make the multiply below a
// runtime soft-float call on every control tick, on a chip with no FPU.
#define ADC_FULL_SCALE_Q8 ((uint32_t)(ADC_MAX_VOLTS * (1L << FRAC_BITS) + 0.5))

uint16_t get_ADC_voltage_Q8(void) {
    uint32_t measured_voltage = (uint32_t)ADC_read() * ADC_FULL_SCALE_Q8;   // Q8, times full scale
    return (uint16_t)(measured_voltage >> ADC_BITS);   // divide by full scale (2^ADC_BITS) -> Q8 volts
}

/* =============================== PROVIDED: PWM init =============================== */

void pwm_init(void) {
    __builtin_write_OSCCONL(OSCCON & 0xbf);  // unlock PPS
    RPOR6bits.RP13R = 13;                    // route OC1 output to RB13 (RP13)
    __builtin_write_OSCCONL(OSCCON | 0x40);  // lock PPS

    OC1RS = PWM_PERIOD;                       // 10 kHz period
    OC1R  = 0;                                // start at 0% duty
    OC1CON2bits.SYNCSEL = 0x1F;               // OC1 is its own sync source
    OC1CON2bits.OCTRIG  = 0;
    OC1CON1bits.OCTSEL  = 0b111;              // clocked from the peripheral clock (Fcy)
    OC1CON1bits.OCM     = 0b110;              // edge-aligned PWM mode (set last, datasheet 15.3)
}


/* =============================== PROVIDED: main() =============================== */

void main(void) {
    ANSELA = 0x0000;              // all PORTA pins digital...
    ANSELB = 0x0000;              // ...all PORTB pins digital
    ANSELAbits.ANSA1  = 1;        // ...except RA1/AN1 = analog input (REQUIRED for the ADC)
    TRISAbits.TRISA1  = 1;        // RA1 as input  (ADC feedback)
    TRISBbits.TRISB13 = 0;        // RB13 as output (PWM)

    timer_init();
    ADC_init();
    pwm_init();

    while (1) {
        if (timer_flag) {                                     // a Timer2 tick is pending
            int32_t measured = get_ADC_voltage_Q8();          // SENSE   (provided wrapper, your ADC_read)
            int32_t error    = REFERENCE - measured;          // COMPARE (provided)
            update_accumulator(error);                        // integral_accumulator is updated here

            // TODO: build the two halves of the PI control signal, each from its own gain:
            //   K_P applied to the error, K_I applied to integral_accumulator. Both halves
            //   stay in Q8 here - set_control_signals() does the single shift back out of Q8.
            //   integral_accumulator is a raw sum of one error sample per tick, so it is
            //   2^SAMPLE_RATE_BITS times larger than the integral it stands for - undo that
            //   scaling here. Scale first and shift second: a right shift throws the low
            //   bits away for good, so shifting before you scale costs you resolution you
            //   cannot get back. Watch the int32 range when you do (see the handout).
            int32_t proportional_action = 0;   // TODO: replace
            int32_t integral_action     = 0;   // TODO: replace

            set_control_signals(proportional_action, integral_action); // PI ACTION (you write) -> set_pwm()
            timer_flag = 0;
        }
        Idle();   // sleep the CPU until the next interrupt (the Timer2 tick wakes it)
    }
}
