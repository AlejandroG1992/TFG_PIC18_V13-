/*
 * __delay32() provides a 32-bit delay routine which will delay for the number
 * of cycles denoted by its argument.  The minimum delay is 12 cycles
 * including call and return statements.  With this function only the time
 * required for argument transmission is not accounted for.  Requests for
 * less than 12 cycles of delay will cause an 12 cycle delay.
 */

extern void __delay32(unsigned long cycles);

/*
 * __delay_ms() and __delay_us() are defined as macros. They depend
 * on a user-supplied definition of FCY. If FCY is defined, the argument
 * is converted and passed to __delay32(). Otherwise, the functions
 * are declared external.
 *
 * For example, to declare FCY for a 10 MHz instruction rate:
 *
#define FCY 10000000UL
 */
#define FCY 4000000UL     // FCY=Fosc/2 y Fosc == 8MHz 
#if !defined(FCY)
extern void __delay_ms(unsigned long);
extern void __delay_us(unsigned long);
#else
#define __delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }
#endif	  continue; }