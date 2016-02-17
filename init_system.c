/*
 * File:   init_system.c
 * Author: Alejandro
 *
 * Created on 5 de febrero de 2016, 17:41
 */


#include "init_system.h"

void init_system(void) {



    // PIC18F26K20 Configuration Bit Settings

    // 'C' source line config statements



    // #pragma config statements should precede project file includes.
    // Use project enums instead of #define for ON and OFF.

    /**         PIC EUSART TEST                *

    /** C O N F I G U R A T I O N   B I T S ******************************/
#pragma config FOSC = INTIO67, FCMEN = OFF, IESO = OFF                        // CONFIG1H                                              // CONFIG7H
#pragma config BOREN = OFF, PWRT = OFF
#pragma config WDTEN = OFF
#pragma config CCP2MX = PORTC, PBADEN = OFF, MCLRE = ON
#pragma config DEBUG = OFF, STVREN = ON, XINST = OFF, LVP = OFF
    // CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

    // CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

    // CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000h-00FFFFh) not write-protected)

    // CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

    // CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

    // CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)



    TRISA = 0b00000011; // RB0 BOTON/ RB4 LED D7/ RB5 LED D8 
    LATA = 0x00; // Reset all Flip-Flops at PORTB
    PORTA = 0x00; // Reset all Flip-Flops at PORTB
    
    TRISB = 0b00001001; // RB0 BOTON/ RB4 LED D7/ RB5 LED D8 
    LATB = 0x00; // Reset all Flip-Flops at PORTB
    PORTB = 0xFF; // Reset all Flip-Flops at PORTB 

    TRISC = 0b11111000; // RB0 BOTON/ RB4 LED D7/ RB5 LED D8 
    LATC = 0x00; // Reset all Flip-Flops at PORTB
    PORTC = 0b00000111; // Reset all Flip-Flops at PORTB



    GIE = 1; // Global interrupts allowed
    PEIE = 1; //Interrupts periferic
    INTCONbits.INT0IF = 0;
    INTCON2bits.INTEDG0 = 0; //INT0 edge selection: '0' --> on falling edge (interrupt after clicking the pushbutton)
    INTCONbits.INT0IE = 1;
    INTCON2bits.RBPU = 0;



    RBIE = 1;
    RBIP = 1;
    OSCCON = 0b01110000;

    InitUSART();
    
    
    
}