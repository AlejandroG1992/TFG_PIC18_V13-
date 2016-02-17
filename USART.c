/*
 * File:   USART.c
 * Author: Alejandro
 *
 * Created on 5 de febrero de 2016, 17:58
 */


#include "USART.h"
void InitUSART(void){
    OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_ON &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX &
            USART_BRGH_HIGH, 103); //9600baudios
    
   
     SendStringUSART("Idle mode ON");
    
}

void SendStringUSART(char* text){
    
    while (BusyUSART());
    putsUSART(text);
    Salto_Enter();
    
    
}
void Salto_Enter(void) {
    while (BusyUSART());
    WriteUSART(0x0D); //Salto de Linea push enter
    while (BusyUSART());
    WriteUSART(0x0A); //    \n
}