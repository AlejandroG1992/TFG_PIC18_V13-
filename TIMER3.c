/*
 * File:   TIMER3.c
 * Author: Alejandro
 *
 * Created on 5 de febrero de 2016, 18:18
 */


#include "TIMER3.h"

void InitializeTimer3(void) {

   TMR3IE = 1;
   T3CON = 0;
   T3CONbits.RD16 = 1;     // 0 = R/W as 8 bits, 1 = R/W as 16 bits
   T3CONbits.T3CCP2 = 0;   // used with both CCP modules
   T3CONbits.T3CKPS1 = 0;  // 2 bit Prescale value:  00: PS = 1,
   T3CONbits.T3CKPS0 = 1;  //   01: PS = 2, 10: PS = 4, 11: PS = 8
   T3CONbits.T3CCP1 = 0;   // used with CCP2 module
   T3CONbits.T3SYNC = 0;   // Timer 1 Osc Sync: 0 = on, 1 = off
   T3CONbits.TMR3CS = 0;   // Clock Source: 0 = Fcy, 1 = Ext Clk
   T3CONbits.TMR3ON = 1;   // 0 = turn timer off, 1 = turn timer on
   TMR3H =  0xF8;           // TMR3H must be written first
   TMR3L =  0x46;           // count up from F830 but there is a small overhead and TMR3L must be 0x46
   TMR3IF = 0;     // reset the interrupt flag    
    
    
}