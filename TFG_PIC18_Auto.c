#include <p18F26K20.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include "I2C1.h"
#include <plib/usart.h>
#include <stdint.h>
#include "DHT22.h"
#include "ADC.h"
#include "init_system.h"
#include "USART.h"
#include "TIMER3.h"

 //dfgdfgdfgdfg

//#define MASTER 0x20						// defines the slave address of this controller
#define SLAVE 0x10						// defines the slave address to be controlled by this controller
#define TIMEOUT 0xfff					// defines the timeout value, must not be too small
#define slave_address 0b10000000
#define Command_Tem 0b11110011
#define Command_Hum 0b11110101

#define Command_TemMCP 0x05
#define AddressByteMCP 0b00110000


//#define AddressByteMCP 0b10010000
//#define Command_TemMCP 0x00

#define 	STATE_Idle		'A'
#define 	STATE_Setup_Pot	'B'
#define 	STATE_Pot            'C'
#define 	STATE_Stop_Pot	'D'
#define     STATE_Setup_TEMP       'E'
#define     STATE_TEMP       'F'
#define     STATE_Stop_TEMP       'G'
#define 	STATE_Setup_ONEWIRE	'H'
#define 	STATE_ONEWIRE         'I'
#define 	STATE_Stop_ONEWIRE	'J'
#define 	STATE_Setup_I2C	'L'
#define 	STATE_I2C         'M'
#define 	STATE_Stop_I2C	'N'
#define 	STATE_Setup_LDR	'P'
#define 	STATE_LDR        'Q'
#define 	STATE_Stop_LDR	'O'




int SerialInterrupt_flag=0;
int Trigger = 0;
unsigned int postscalermax = 1000; //////////// el led se enciende y se apaga cada un segundo
unsigned int postscaler30smax=8000;
unsigned int postscalerflag30s=0;
unsigned int postscaler30s = 0;
unsigned int postscaler = 0;
unsigned int postscalerflag = 0;
unsigned int postscalersampleflag=0;
unsigned int postscalersample=0;
unsigned int postscalersamplemax=250;

static unsigned int Value_AN0 = 0; ///potenciometro
static unsigned int Value_AN1 = 0; // sensor de temperatura
static unsigned int Value_AN9 = 0; // sensor LDR

unsigned int Valor_Medio_HumedadTierra[4];
float Valor_Medio_TempAnalog[4];
unsigned int Valor_Medio_HT=0;
float Valor_Medio_TA=0;
unsigned int samples_num = 0;


char dataI2C_Hum;
char dataI2C_Tem;
int32_t I2C_Tem = 0;
int32_t I2C_Hum = 0;
float I2C_TemFormula = 0.0;
float I2C_HumFormula = 0.0;
unsigned int I2C_dev=0;
unsigned int I2C_manu = 0;



static float Vdd = 3.335; ///3.5V
static float Vo = 0.505; ///0ºC 0.5V
static float sensibilidad = 0.01; //10mv/ºC
float Value_temp = 0.0;
float Value_pot = 0.0;

char CharRX;
char CharTX[] = "Hello World";
char CharTX2[] = "Hello World";



void __interrupt IntServe();
char output_logic(void);
char state_logic(void);
static char present_state = STATE_Idle; // state variable

void delay_ms(unsigned long ms);


int main(void) {
    init_system();
    while (1) {
         
        output_logic();
        state_logic();
        if(SerialInterrupt_flag==1){
            present_state = CharRX;
            SerialInterrupt_flag=0;
        }

    }
}

char state_logic(void) {

    char error = 0;

    switch (present_state) {
        case STATE_Idle:
            //IFS0bits.INT0IF = 1;
            if (Trigger == 1) {
                // if a button pressed is detected,
                present_state = STATE_Setup_Pot;
                Trigger = 0;
                // IFS0bits.INT0IF = 0;
            } else {
                present_state = STATE_Idle;
            }
            //present_state = Setup_Pot;
            break;
        case STATE_Setup_Pot:
            present_state = STATE_Pot;

            break;
        case STATE_Pot:
            if (Trigger == 1) {
                // if a button pressed is detected,
                present_state = STATE_Stop_Pot;
                Trigger = 0;
            } else {
                present_state = STATE_Pot;
            }
            break;


        case STATE_Stop_Pot:
            present_state = STATE_Setup_TEMP;

            break;



        case STATE_Setup_TEMP:
            present_state = STATE_TEMP;
            break;

        case STATE_TEMP:
            if (Trigger == 1) {
                // if a button pressed is detected,
                present_state = STATE_Stop_TEMP;
                Trigger = 0;
                // IFS0bits.INT0IF = 0;
            } else {
                present_state = STATE_TEMP;
            }

            //present_state = STATE_Stop_TEMP;
            break;

        case STATE_Stop_TEMP:

            present_state = STATE_Setup_ONEWIRE;
            break;

        case STATE_Setup_ONEWIRE:
            present_state = STATE_ONEWIRE;
            break;

        case STATE_ONEWIRE:
            if (Trigger == 1) {
                
                // if a button pressed is detected,
                present_state = STATE_Stop_ONEWIRE;
                Trigger = 0;
                // IFS0bits.INT0IF = 0;
            } else {
                present_state = STATE_ONEWIRE;
            }

            break;

        case STATE_Stop_ONEWIRE:
            present_state = STATE_Setup_I2C;

            break;
        case STATE_Setup_I2C:
            present_state = STATE_I2C;
            break;

        case STATE_I2C:
            if (Trigger == 1) {
                // if a button pressed is detected,
                present_state = STATE_Stop_I2C;
                Trigger = 0;
                // IFS0bits.INT0IF = 0;
            } else {
                present_state = STATE_I2C;
            }
            break;

        case STATE_Stop_I2C:
            present_state = STATE_Setup_LDR;
            break;
    
        case STATE_Setup_LDR:
            present_state = STATE_LDR;
            break;


        case STATE_LDR:
            if (Trigger == 1) {
                // if a button pressed is detected,
                present_state = STATE_Stop_LDR;
                Trigger = 0;
                // IFS0bits.INT0IF = 0;
            } else {
                present_state = STATE_LDR;
            }
            break;


        case STATE_Stop_LDR:
            present_state = STATE_Idle;
            break;    
           
        default:
            error = 1;
    }
    return (error);
}

char output_logic(void) {
    char error = 0;
    switch (present_state) {
        case STATE_Idle:
            PORTC = 0b00000111; // Reset all Flip-Flops at PORTB

            if (postscalerflag30s == 1) {
                postscalerflag30s =0;
                Trigger = 1;
            }
            break;

        case STATE_Setup_Pot:
            T3CON = 0;
            ADON = 0;
            CloseI2C();
            
            InitializeTimer3(); //interrupt every second
            InitADC();
            
            postscaler = 0;
            postscalerflag = 0;
            
            SendStringUSART("Analog Mode Acquiring");
            
            PORTC = 0b00000101; // Reset all Flip-Flops at PORTB
            SelectChannelADC(0);//select AN0
            samples_num = 0;
            Valor_Medio_HT=0;
            break;
        case STATE_Pot:
            
            if (postscalersampleflag == 1) {
                Value_AN0=0;
                Value_AN0 = GetSampleADC();
                Valor_Medio_HumedadTierra[samples_num]= Value_AN0;
                
                
                
                postscalersampleflag = 0;
                
                samples_num++;
                
            }
            if(samples_num == 3){
                samples_num=0;
                while(samples_num < 4){
                
                Valor_Medio_HT = Valor_Medio_HT + Valor_Medio_HumedadTierra[samples_num];
                
                
                samples_num++;
                }
                Valor_Medio_HT = Valor_Medio_HT/4;
                sprintf(CharTX, "DATA ANALOG :Humedad TIERRA = %d",Valor_Medio_HT);
                SendStringUSART(CharTX);
                Trigger =1;
                
                
            }


            break;
        case STATE_Stop_Pot:
            postscalersampleflag = 0;
            break;
  
        case STATE_Setup_TEMP: 
            SendStringUSART("I2C Mode Acquiring");
            PORTC = 0b00000100; // Reset all Flip-Flops at PORTB
            
            SelectChannelADC(1);//select AN1
            
            samples_num=0;
            Valor_Medio_TA =0;
            break;

        case STATE_TEMP:
            if (postscalersampleflag == 1) {
                Value_AN1=0;
                
                
                Value_AN1 = GetSampleADC();
                ////Value_temp = (float) ((((Value_AN1 / 1023.0) * Vdd - Vo)) / sensibilidad); /// Sensor Placa XLP           
                Value_temp = (float)(log(10000.0 * ((1024.0 / Value_AN1 - 1))));
                Value_temp = (float)(1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Value_temp * Value_temp )) * Value_temp ));
                Value_temp = (float)(Value_temp - 273.15);            // Convert Kelvin to Celcius
                //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
                Valor_Medio_TempAnalog[samples_num]=Value_temp; 
                
                
                if(Value_pot < 50){
                    PORTBbits.RB5 =1;
                }
                else{
                    PORTBbits.RB5 =0;
                }

                
                
               
                
                samples_num++;
                postscalersampleflag = 0;

                
            }
            if(samples_num == 4){
                samples_num=0;
                while(samples_num < 4){
                
                Valor_Medio_TA = Valor_Medio_TA + Valor_Medio_TempAnalog[samples_num];
                
                
                samples_num++;
                }
                Valor_Medio_TA = Valor_Medio_TA/4.0;
                sprintf(CharTX, "DATA ANALOG :Temperature = %.1f C",Valor_Medio_TA);
                SendStringUSART(CharTX);
                Trigger =1;
                
                
            }
            
            
            break;

        case STATE_Stop_TEMP:
            postscalersampleflag = 0;
            break;
        case STATE_Setup_ONEWIRE:
            PORTC = 0b00000111; // Reset all Flip-Flops at PORTB
            TRISBbits.TRISB4 = 0;
            PORTBbits.RB4 = 1;
            TMR2IE = 1;       // Enable Timer2 interrupt
            T2CON = 0;        // Prescaler 1:1, and Timer2 is off initially  
            TMR2IF = 0;       // Clear TMR INT Flag bit
            TMR2 = 0;
            T2CONbits.T2CKPS = 0b01;
            break;

        case STATE_ONEWIRE:

            GetDataSensorDHT22();

            if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)) {
                if (sign) {
                    sprintf(CharTX, "DATA DHT22 :Temperature = -%.1f C", Value_tempdht22);

                    SendStringUSART(CharTX);
                } else {
                    sprintf(CharTX, "DATA DHT22 :Temperature = %.1f C", Value_tempdht22);
                    SendStringUSART(CharTX);
                }

                sprintf(CharTX, "DATA DHT22 :Humidity = %.1f %%", Value_rhdht22);
                SendStringUSART(CharTX);

            } else {
                SendStringUSART("Checksum Error!   Trying Again ... ");
            }
            Trigger = 1;

            break;

        case STATE_Stop_ONEWIRE:
            break;
            

        case STATE_Setup_I2C:           
            Open_I2C1();
            break;
            
        case STATE_I2C:
            
           if (postscalerflag == 1) {
               // I2C_dev = WriteDEVICE_I2C(AddressByteMCP, 0x07);
               // I2C_manu = WriteMANU_I2C(AddressByteMCP, 0x06);
                
                I2C_TemFormula = ReadByte_I2C(AddressByteMCP, 0x05); //temperature


                sprintf(CharTX2, "DATA I2C :Temperature = %.1f C", I2C_TemFormula);

                SendStringUSART(CharTX2);
                
                postscalerflag = 0;
                Trigger = 1;
            }
            
            break;

        case STATE_Stop_I2C:
                      
            SendStringUSART("LDR mode ON");
            
            break;


        case STATE_Setup_LDR:
            //InitializeTimer3(); //interrupt every second
            SelectChannelADC(9);//select AN9
 
            break;

        case STATE_LDR:

           if (postscalerflag == 1) {
                Value_AN9 = 0;
                
                Value_AN9 = GetSampleADC();
                
                sprintf(CharTX, "DATA LDR :Valor Analog = %d",Value_AN9);
                
                SendStringUSART(CharTX);
                
                postscalerflag = 0;
                Trigger = 1;
            }
            
            break;

        case STATE_Stop_LDR:

            SendStringUSART("Sleep 8s");
            
            CloseI2C();
            T2CON = 0;
            StopADC();
            SendStringUSART("Idle mode ON");
            
            break;    
  
        default:
            error = 1;
    }

    return (error);
}

void __interrupt IntServe(void) {

    GIE = 0; //Disable interrupts while attending one of them ...

    if (RCIF == 1) {
        CharRX = ReadUSART();
        //present_state = CharRX;
        SerialInterrupt_flag=1;

        //while (BusyUSART());
        //WriteUSART(CharRX);


        RCIF = 0;
    }

    if (INT0IF == 1) {

        Trigger = 1;
        INT0IF = 0;
    }
    if (TMR3IF == 1) {
       TMR3H =  0xF8;           // TMR3H must be written first
       TMR3L =  0x46;           // count up from F830 but there ara a small overhead and TMR3L must be 0x46
        if (postscaler < postscalermax) {
            postscaler++;
        } else {
            postscaler = 0;
            postscalerflag = 1;
        }
        if ((postscalersample < postscalersamplemax) && (present_state==STATE_TEMP || present_state==STATE_Pot)) {
            postscalersample++;
            
        } else {
            postscalersample = 0;
            postscalersampleflag = 1;
        }
        if (postscaler30s < postscaler30smax) {
            postscaler30s++;
        } else {
            
            postscaler30s=0;
            postscalerflag30s=1;
        }
        
       TMR3IF = 0;
       
       
    }
    if (TMR2IF == 1) {
        //Trigger = 1;
      if (present_state == STATE_ONEWIRE) {  
        TOUT = 1;
        TMR2ON = 0; // stop timer
        TMR2IF = 0; // Clear TMR0 interrupt flag
      }
        
        TMR2IF = 0;
    }
    GIE = 1; //Disable interrupts while attending one of them ...
}









