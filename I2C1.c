#include "I2C1.h"
void Open_I2C1(void)
{
    
    
    //EnableIntI2C;
    SSPADD= 0x27;
    OpenI2C(MASTER, SLEW_OFF);///0x4E
    
    IdleI2C();

}



unsigned int WriteMANU_I2C(unsigned char ReadAddressIC, unsigned char RegisterAddress)
{
     unsigned char UpperByte = 0;
    unsigned char LowerByte = 0;
    unsigned int tmp = 0;
    ResetVariables_I2C();
    StartI2C();
    
    WaitFlag();

    //Write Slave address and set master for transmission
    WriteI2C(ReadAddressIC);  
    WaitFlag();
    
     WaitForACK();

    WriteI2C(RegisterAddress);
    WaitFlag();
    WaitForACK();
    __delay_ms(45);
    __delay_ms(45);
    ResetVariables_I2C();
    RestartI2C();
    WaitFlag();
    
    WriteI2C(ReadAddressIC + 1);
    WaitFlag();
    AckI2C();
    ResetVariables_I2C();

    UpperByte = ReadI2C();
    WaitFlag();
    AckI2C();
    ResetVariables_I2C();
    
    LowerByte = ReadI2C();
    WaitFlag();
    ResetVariables_I2C();
    
    NotAckI2C();
    WaitFlag();
    ResetVariables_I2C();
    StopI2C();
   WaitFlag();
    IdleI2C();
    
    tmp = UpperByte;
    tmp <<= 8;
    tmp |=LowerByte;
    //Temperature = (float)(((UpperByte*16.0 + LowerByte) / 16.0));
    
    return tmp;
    
}
unsigned int WriteDEVICE_I2C(unsigned char ReadAddressIC, unsigned char RegisterAddress)
{
        unsigned char UpperByte = 0;
    unsigned char LowerByte = 0;
    unsigned int tmp = 0;
    ResetVariables_I2C();
    StartI2C();
    
    WaitFlag();

    //Write Slave address and set master for transmission
    WriteI2C(ReadAddressIC);  
    WaitFlag();
    
     WaitForACK();

    WriteI2C(RegisterAddress);
    WaitFlag();
    WaitForACK();
    __delay_ms(45);
    __delay_ms(45);
    ResetVariables_I2C();
    RestartI2C();
    WaitFlag();
    
    WriteI2C(ReadAddressIC + 1);
    WaitFlag();
    WaitForACK();
    ResetVariables_I2C();

    UpperByte = ReadI2C();
    WaitFlag();
    AckI2C();
    ResetVariables_I2C();
    
    LowerByte = ReadI2C();
    WaitFlag();
    ResetVariables_I2C();
    
    NotAckI2C();
    WaitFlag();
    ResetVariables_I2C();
    StopI2C();
   WaitFlag();
    IdleI2C();
    
    
    
    
    tmp = UpperByte;
    tmp <<= 8;
    tmp |=LowerByte;
    //Temperature = (float)(((UpperByte*16.0 + LowerByte) / 16.0));
    
    return tmp;
    
    
}

float ReadByte_I2C (unsigned char ReadAddressIC, unsigned char RegisterAddress)
{
    unsigned char UpperByte = 0;
    unsigned char LowerByte = 0;
    unsigned int tmp = 0;
    float Temperature = 0;
    ResetVariables_I2C();
    StartI2C();
    
    WaitFlag();

    //Write Slave address and set master for transmission
    WriteI2C(ReadAddressIC);  
    WaitFlag();
    
     WaitForACK();

    WriteI2C(RegisterAddress);
    WaitFlag();
    WaitForACK();
    __delay_ms(45);
    __delay_ms(45);
    ResetVariables_I2C();
    RestartI2C();
    WaitFlag();
    
    WriteI2C(ReadAddressIC + 1);
    WaitFlag();
    WaitForACK();
    ResetVariables_I2C();

    UpperByte = ReadI2C();
    WaitFlag();
    AckI2C();
    ResetVariables_I2C();
    
    LowerByte = ReadI2C();
    WaitFlag();
    ResetVariables_I2C();
    
    NotAckI2C();
    WaitFlag();
    ResetVariables_I2C();
    StopI2C();
   WaitFlag();
    IdleI2C();

    if ((UpperByte & 0x80) == 0x80) { //TA ³ TCRIT
    }
    if ((UpperByte & 0x40) == 0x40) { //TA > TUPPER
    }
    if ((UpperByte & 0x20) == 0x20) { //TA < TLOWER
    }
    UpperByte = UpperByte & 0x1F; //Clear flag bits
    if ((UpperByte & 0x10) == 0x10) { //TA < 0°C
        UpperByte = UpperByte & 0x0F; //Clear SIGN
        Temperature = (float)(256.0 - (UpperByte * 16.0 + LowerByte / 16.0));
    } else //TA ³ 0°C
        Temperature = (float)((UpperByte * 16.0 + LowerByte / 16.0));
//    tmp = UpperByte;
//    tmp <<= 8;
//    tmp |=LowerByte;
    //Temperature = (float)(((UpperByte*16.0 + LowerByte) / 16.0));
    //Temperature = (float)(((tmp) / 1.0));
    return Temperature;

}

void ResetVariables_I2C(void)
{
    SSPCON2bits.ACKEN=0;
    SSPCON2bits.PEN=0;
   SSPCON2bits.RCEN=0;
    SSPCON2bits.RSEN=0;
    SSPCON2bits.SEN=0;
}

void WaitFlag(void)
{
    while(!PIR1bits.SSPIF);           // wait for flag to be high
    PIR1bits.SSPIF=0;
}
void WaitForACK(void)
{
    while(SSPCON2bits.ACKSTAT);        // wait for ack receive from slave
}
