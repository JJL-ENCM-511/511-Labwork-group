/*
 * File:   uart.c
 * Author: psbta
 *
 * Created on September 27, 2025, 8:07 PM
 */


#include "uart.h"
#include <stdint.h>

uint8_t received_char = 0;
uint8_t RXFlag = 0;

void InitUART2(void) 
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS before remapping pins
    RPINR19bits.U2RXR = 11; // U2RX  <- RP11
    RPOR5bits.RP10R = 5;    // U2TX  -> RP10 (RB10)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS again

    U2MODE = 0b0000000010001000;

    U2BRG = 103;            // ??????????????????????
    
	U2STAbits.UTXISEL0 = 0;
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXISEL = 0b00;

	IFS1bits.U2TXIF = 0;	
    IPC7bits.U2TXIP = 3; 
    
	IEC1bits.U2TXIE = 0; 
	IFS1bits.U2RXIF = 0; 
	IPC7bits.U2RXIP = 4; 
    IEC1bits.U2RXIE = 1;

	U2MODEbits.UARTEN = 1;	

	U2STAbits.UTXEN = 1;
	return;
}

void Disp2String(char *str) //Displays String of characters
{
    unsigned int i;
    for (i=0; i<= strlen(str); i++)
    {
        XmitUART2(str[i],1);
    }

    return;
}

void XmitUART2(char CharNum, unsigned int repeatNo)
{	
	U2STAbits.UTXEN = 1;
	while(repeatNo!=0) 
	{
		while(U2STAbits.UTXBF==1)   // We loop here because.... 
		{
		}	
		U2TXREG=CharNum;            // We put XXXXXX here because .....
		repeatNo--;
	}
	while(U2STAbits.TRMT==0)        // We check XXXXX because .........
	{
	}

    U2STAbits.UTXEN = 0;
}

/************************************************************************
 * Receive a buf_size number of characters over UART
 * Description: This function allows you to receive buf_size number of characters from UART,
 * provided the UART module has been initialized correctly. The function currently returns
 * if the "enter" key (ASCII 0x0D) is received. The function does not handle receiving
 * the DELETE or BACKSPACE keys meaningfully. 
 * 
 * Note: there is commented-out skeleton code that could be (re)implemented to allow the function
 * return early without the ENTER key given an interrupt-set global flag. 
 ************************************************************************/
void RecvUart(char* input, uint8_t buf_size)
{	
    uint16_t i = 0;
    char last_char;
    // wait for enter key
    while (last_char != 0x0D) {
        if (RXFlag == 1) {
            // only store alphanumeric characters
            if (received_char >= 32 && received_char <= 126) {
                if (i > buf_size-2) {
                    Disp2String("\ntoo long\n\r");
                    RXFlag = 0;
                    return;
                }
                input[i] = received_char;
                i++;
                XmitUART2(received_char,1); // loop back display
                U2STAbits.OERR = 0;
            }
            last_char = received_char;
            RXFlag = 0;
        }
        // wait for next character
        
        // if (CNflag == 1) { // this allows breaking out of the busy wait if CN interrupts are enabled...
        //     add logic here
        // }
    }
}

/************************************************************************
 * Receive a single (alphanumeric) character over UART
 * Description: This function allows you to receive a single character, which will only be 
 * "accepted" (returned) after receiving the ENTER key (0x0D). 
 * While receiving characters, the program is designed to send back the received character.
 * To display this, it sends a BACKSPACE (0x08) to clear the previous character from the 
 * receiving terminal, before sending the new current character. 
 * 
 * Note: there is commented-out skeleton code that could be (re)implemented to allow the function
 * return early without the ENTER key given an interrupt-set global flag. 
 ************************************************************************/
char RecvUartChar()
{	
    char last_char;
    XmitUART2(' ',1);
    // wait for enter key
    while (last_char != 0x0D) {
        if (RXFlag == 1) {
            
            // return the last character received if you see ENTER
            if (received_char == 0x0D) {
                RXFlag = 0;
                return last_char;
            }
            
            // only store alphanumeric characters
            if (received_char >= 32 && received_char <= 126) {
                XmitUART2(0x08,1); // send backspace
                last_char = received_char;
                XmitUART2(received_char,1); // loop back display
            }
           
            U2STAbits.OERR = 0;
            RXFlag = 0;
        }
        
        // if (CNflag == 1) { // this allows breaking out of the busy wait if CN interrupts are enabled...
        //     add logic here
        // }
    }
}

/*
* @brief Transmits a 16-bit number using UART TX pin
*
* @param data Data to be sent
*/
void lab_511_send_number(uint16_t data) {
    char hNum, lNum;
    hNum = (char)(data >> 8); lNum = (char)(data & 0x00FF);

    char strBuf[5];
    /* Big endian */
    uint16_t num = data;
    uint16_t len = 0;
    while(num != 0) {
        strBuf[len++] = '0' + (num % 10);
        num = num / 10;
    }

    uint16_t mid = len/2;
    for(int i = 0; i < mid; i++) { /* Reversing array */
        char temp = strBuf[len-i-1];
        strBuf[len-i-1] = strBuf[i];
        strBuf[i] = temp;
    }

    for(int i = 0; i < 5 - len; i++) { /* Zero pad */
        XmitUART2('0',1);
    }
    for(int i = 0; i < len; i++) { /* Send ASCII data */
        XmitUART2(strBuf[i],1);
    }
    XmitUART2('\n',1);
}

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    
	IFS1bits.U2RXIF = 0;
    
    received_char = U2RXREG;
    
    RXFlag = 1;
    
    _LATB5 ^= 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
	IFS1bits.U2TXIF = 0;

}