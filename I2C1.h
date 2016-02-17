/*File:   I2C1.h
Author: IBRAHIM NAZIR SANGI

Created on July 6, 2013, 7:35 AM
 */
 #define I2C_V1
#ifndef I2C1_H
#define	I2C1_H   

#include <pconfig.h>
#include <p18F26K20.h>
#define _XTAL_FREQ 16000000
//#include <plib/i2c.h>
#include "i2c.h"
#ifdef	__cplusplus
extern "C" {
#endif


    void Open_I2C1(void);
    unsigned int  WriteDEVICE_I2C(unsigned char ReadAddressIC, unsigned char RegisterAddress);
    unsigned int  WriteMANU_I2C(unsigned char ReadAddressIC, unsigned char RegisterAddress);
    float ReadByte_I2C (unsigned char ReadAddressIC, unsigned char RegisterAddress);
    void ResetVariables_I2C(void);
    void WaitFlag(void);
    void WaitForACK(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C1_H */

