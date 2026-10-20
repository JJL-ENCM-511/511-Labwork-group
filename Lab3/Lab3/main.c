/*
 * File:   main.c
 * Author: Joshua Prud'Homme, Luke Zenha, Johann Yap
 *
 * Created FOR ENCM 511
 * PLEASE ADD DATE CREATED HERE: 2026-10-09
 * 
 * FAILURE TO UPDATE THIS HEADER WITH YOUR GROUP MEMBER NAMES
 * MAY RESULT IN PENALTIES
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
#include "uart.h"
#include "config.h"
#include "delay_ms.h"
#include "uart_line.h"
#include "blink.h"

#define TESTSTATE



uint16_t slow = 0;
char received;

//FSM state definitions
typedef enum {
    Fast_Idle,      // Default fast mode state, start-up state, no blinking, idle();
    Fast_PB0,       // PB0 pressed, blinks @0.25s (goes back to idle after PB1 pressed again)
    Fast_PB1,       // PB1 pressed, blinks @0.5s (goes back to idle after PB1 pressed again)
    Fast_PB2,       // PB2 pressed, blinks @1s (goes back to idle after PB2)
    Fast_DualPB,    // 2 PBs are pressed @same time in fast mode, stays on (goes back to idle after any PB is pressed)
    Prog_Idle,      // Default prog mode state, no blinking, idle();
    Prog_PB0,       // PB0 pressed, blinks @3s (goes back to idle after PB0 pressed again)
    Prog_ModeUse,   // PB1 pressed, blinks time selected by Prog_ModeSel input (0,1,2), default to state 0 [State 0: 0.25s, State 1: 0.5s, State 2: 1s]
    Prog_ModeSel,   // PB2 pressed, waits for user input to select Prog_ModeUse blink time state, Also blinks @0.125s
} state;

state current_mode = Fast_Idle; // Default state is Fast_Idle
state event_state = Fast_Idle;   // Default event state is Fast_Idle

int switch_state_flag = 0;
int blink_setting_speed = 2; // Default blink setting speed is 2 (0.25s) for Prog_ModeUse state


int main(void) {
    
     //Below initalization functions are from config.c file
    IOinit();
    IOCconfig();
    T2config();
    T3config();

    _LATB5 = 1;
    
    InitUART2();
    
    while(1) {
/*
#ifdef TESTSTATE     // Testing State Switching Code
        ...
#endif
*/
#ifndef TESTSEND    // Actual assignment code

        // Below is logic to determine which state FSM is in based on software flags triggered by the interrupt
        
        while(1) {
        
        // Placed at top level 
        if(PB0_event && PB1_event && PB2_event) {     // All 3 PBs pressed = switch state and ignore other possible events
            PB0_event = 0; // Reset PB0 trigger
            PB1_event = 0; // Reset PB1 trigger 
            PB2_event = 0; // Reset PB2 trigger
            switch_state_flag = 1; // Set flag to switch state
        
            if(current_mode == Fast_Idle){
                current_mode = Prog_Idle;
                event_state = Prog_Idle;
            }
            else{
                current_mode = Fast_Idle;
                event_state = Fast_Idle;
            }
        }

        if(PB0_event) {     // Blinks for 0.25s, or 3s depending on mode
            PB0_event = 0; // Reset PB0 trigger
            if(current_mode == Fast_Idle) {     //Check current mode
                if(event_state == Fast_PB0 || Fast_DualPB) {   //If it's already in state switch it to idle
                    event_state = Fast_Idle;
                }
                else 
                event_state = Fast_PB0; //Otherwise make that event
            }
            else if(current_mode == Prog_Idle) {
                if(event_state == Fast_PB0) {   
                    event_state = Prog_Idle;
                }
                else 
                event_state = Prog_PB0;
            }
        }

        if(PB1_event) {     // Blinks for 0.5s or Xs (default 0.25s) depending on mode and previously selected setting
            PB1_event = 0; // Reset PB1 trigger
            if(current_mode == Fast_Idle) {     //Check current mode
                if(event_state == Fast_PB1 || Fast_DualPB) {   //If it's already in state switch it to idle
                    event_state = Fast_Idle;
                }
                else 
                event_state = Fast_PB1; //Otherwise make that event
            }
            else if(current_mode == Prog_Idle) {
                if(event_state == Prog_ModeUse) {   
                    event_state = Prog_Idle;
                }
                else 
                event_state = Prog_ModeUse;
            }
        }

        if(PB2_event) {
            PB2_event = 0; // Reset PB2 trigger
            if(current_mode == Fast_Idle) {     //Check current mode
                if(event_state == Fast_PB2 || Fast_DualPB) {   //If it's already in state switch it to idle
                    event_state = Fast_Idle;
                }
                else 
                event_state = Fast_PB2; //Otherwise make that event
            }
            else if(current_mode == Prog_Idle) {
                if(event_state == Prog_ModeSel) {   
                    event_state = Prog_Idle;
                }
                else 
                event_state = Prog_ModeSel;
            }
        }

        if(dual_PB_event && !switch_state_flag) {     // 2 PBs pressed at same time, stays on until any PB is pressed again, checks if switch_state mode is on
            dual_PB_event = 0; // Reset dual PB trigger, Placed at end to overwrite any previous states 
            if(current_mode == Fast_Idle) 
                event_state = Fast_DualPB;
        }

        if(switch_state_flag)     // Reset switch state flag after switching states
                                  // Used so that system doesn't get confused and think all 3 PB's pressed is Dual_PB mode
            switch_state_flag = 0;
        

        switch(event_state) {
            case Fast_Idle:
                led0_blink(0);
                break;

            case Prog_Idle:
                led0_blink(0);
                break;

            case Fast_PB0:
                led0_blink(2);
                break;

            case Fast_PB1:
                led0_blink(4);
                break;

            case Fast_PB2:
                led0_blink(8);
                break;

            case Fast_DualPB:
                led0_blink(-1);
                break;

            case Prog_PB0:
                led0_blink(24);
                break;  

            case Prog_ModeUse:
                led0_blink(blink_setting_speed);
                break;
            
            case Prog_ModeSel:
                //HIGHLY experimental
                //F&!king around with bit shifts. Remove comment if it works.
                blink_setting_speed = 1LL << ((BlinkSettingUart() - 48;) + 1);

                break;    
        }

        Idle();
    }
#endif
        
    }
    

return 0;
}


