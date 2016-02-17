#include "DHT22.h"

void start_signal()
 { DataDir = 0;     // Data port is output
   Data    = 0;
   __delay_ms(18);
   DataDir = 1;     // Data port is input
   __delay_us(30);
 }
unsigned short check_response()
 { TOUT = 0;
   TMR2 = 0;
   TMR2ON = 1;                // start timer
   while (!Data && !TOUT);
   if (TOUT) 
      return 0;
   else 
    { TMR2 = 0;
      while (Data && !TOUT);
      if (TOUT) 
         return 0;
      else 
       { TMR2ON = 0;
         return 1;
       }
    }
 }


unsigned short read_byte()
 { unsigned short num = 0;
   DataDir = 1;
   for (i=0; i<8; i++)
    { while (!Data && !TOUT);
      TMR2 = 0;
      TMR2ON = 1;
      while (Data && !TOUT);
      TMR2ON = 0;
      if (TMR2 > 49) 
         num |= 1<<(7-i);  // si el tiempo > 49us, Data es 1
    }
   return num;
 }
void GetDataSensorDHT22(void){
    char CharTX[];
    start_signal();
            check = check_response();
            
            if (!check) 
       { 
                 
            SendStringUSART("No response  from the sensor DHT22");    
              
       }
      else
       { 
         RH_Byte1 = read_byte();
         RH_Byte2 = read_byte();
         T_Byte1  = read_byte();
         T_Byte2  = read_byte();
         CheckSum = read_byte();
         __delay_us(50);
         TRISBbits.TRISB4 = 0;
            PORTBbits.RB4 = 1;
         // Check for error in Data reception
            
         rh   = RH_Byte2 | (RH_Byte1<<8);
         temp = T_Byte2  | (T_Byte1<<8);
         sign = 0;
         if (temp>0x8000)
          { temp &= 0x0fff;
            sign = 1;
           }
         Value_rhdht22 = (float)(rh/10.0);
         Value_tempdht22 = (float)(temp/10.0);
//         if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF))
//          { 
//            if (sign){
//                sprintf(CharTX, "DATA DHT22 :Temperature = -%.1f C", Value_tempdht22);
//                
//            SendStringUSART(CharTX);
//            }
//               
//            else{
//                 sprintf(CharTX, "DATA DHT22 :Temperature = %.1f C", Value_tempdht22);
//            SendStringUSART(CharTX);
//            }
//               
//            
//            
//            
//            sprintf(CharTX, "DATA DHT22 :Humidity = %.1f %%", Value_rhdht22);
//            
//            SendStringUSART(CharTX);
//            
//          }
//         else
//          { 
//             SendStringUSART("Checksum Error!   Trying Again ... ");
//             
//
//          }
 }
        
            
}