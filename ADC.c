/*
 * File:   ADC.c
 * Author: Alejandro
 *
 * Created on 5 de febrero de 2016, 17:26
 */


#include "ADC.h"

void InitADC(){
    
    ADON = 1;
    ADFM = 1;
    
}
unsigned int GetSampleADC(){// sensor LDRInitADC(){
    
    GO = 1;
    while (DONE) {
        ;
    }//aqui pongo el while para que capture
    Value_AD = ADRES;
    
    return Value_AD;
}
void SelectChannelADC(int channel){
    ADCON0bits.CHS = channel;
}

void StopADC(){
    
    ADON = 0;
    
}