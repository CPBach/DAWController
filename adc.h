#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

extern void ADC_Init( void );
extern unsigned char ADC_Read( uint8_t channel );
extern unsigned char avg_metering( unsigned char channel, unsigned char count );


#endif /* ADC_H_ */
