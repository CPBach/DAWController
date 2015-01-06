/*
 * main.c
 *
 *  Created on: 24.12.2014
 *      Author: stefan
 */



#include <avr/io.h>
#include "lcd.h"

#define RUNNING 1
#define SUCCESS 0


#define MULTIPLEXED_INPUTS 0x07
#define MULTIPLEXED_PORT PORTA

#define USART_BAUDRATE 31250
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#ifdef LCD_PORT
#define DISPLAY_PORT LCD_PORT
#endif

void ADC_Init( void );
uint16_t ADC_Read( uint8_t channel );
void MIDI_init( void );

int main ( void ){

	// Initialize Display
	lcd_init(LCD_DISP_ON);

	// Display title
	char* headtitle = "DAWController1.0";
	lcd_puts(headtitle);


	// Define Input
	DDRA = 0x00;
	PORTA |= 0x00;
	// Define Outputs
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;


	// Init AD converter

	ADC_Init();


	while(RUNNING){
		lcd_putc(ADC_Read(0) & 0xff);
	}

	return SUCCESS;
}


void ADC_Init( void ){
	// Initialize AD-converter
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
	ADCSRA |= (1<<ADEN);                  // ADC aktivieren
	ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
	while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
	}
}

uint16_t ADC_Read( uint8_t channel )
{
  // Kanal waehlen, ohne andere Bits zu beeinflußen
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
  ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
  while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
  }
  return ADCW;                    // ADC auslesen und zurückgeben
}

void MIDI_init( void ){
	// 31,250 baud, asynchronous, 1 start bit, 1 stop bit



	// Use 8-bit character sizes, 1 start, 1 stop bit - URSEL bit set to select the UCRSC register
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);

	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;
	// Turn on the transmission circuitry
	UCSRB = (1 << TXEN);
}

void uart_put(char s)
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) );
// you could also write:
// loop_until_bit_is_set(UCSRA,UDRE);
/* Put data into buffer, sends the data */
UDR = s;
}
