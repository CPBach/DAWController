/*
 * adc.c
 *
 *  Created on: 07.01.2015
 *      Author: Bubble
 */

#include "adc.h"

void ADC_Init( void ){
	// Initialize AD-converter
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
	ADCSRA |= (1<<ADEN);                  // ADC aktivieren
	ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
	while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
	}
}

unsigned char ADC_Read( uint8_t channel )
{
  // Kanal waehlen, ohne andere Bits zu beeinflußen
  char tmp;
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
  ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
  while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
  }

  tmp = (unsigned char)(ADCW >> 3);

  return tmp;                    // ADC auslesen und zurückgeben
}


unsigned char avg_metering( unsigned char channel, unsigned char count){
	uint16_t tmp = 0;
	// Drop first reading
	ADC_Read(channel);
	for(int i=0;i<count;i++){
		tmp += ADC_Read(channel);
	}
	tmp /= count;
	return (unsigned char) tmp;
}
