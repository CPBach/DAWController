/*
 * main.c
 *
 *  Created on: 24.12.2014
 *      Author: stefan
 */



#include <avr/io.h>
#include<avr/interrupt.h>
#include<avr/sleep.h>
#include "lcd.h"
#include "midi.h"

#define RUNNING 1
#define SUCCESS 0


#define MULTIPLEXED_INPUTS 0x07
#define MULTIPLEXED_PORT PORTA

#define CONTROL_PORT PORTB


#ifdef LCD_PORT
#define DISPLAY_PORT LCD_PORT
#endif


void ADC_Init( void );
void MIDI_init( void );
void TIMER_init( void );

unsigned char ADC_Read( uint8_t channel );
unsigned char avg_metering( unsigned char channel, unsigned char count );



volatile unsigned char output_state = 0x00;
volatile unsigned char channel = 0x00;

volatile unsigned char values[3][16];
char* char_to_number(unsigned char c);

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

	// Initialize all units
	ADC_Init();
	MIDI_init();
	TIMER_init();


	while(RUNNING){
		unsigned char tmp_output_state = output_state;

		unsigned char reading = avg_metering(channel, 16);

		if (values[channel][output_state] != reading){
			values[channel][output_state] = reading;
			midi_send(0xB0);
			midi_send(0);
			midi_send(127);
			lcd_gotoxy(0,1);
			lcd_puts(mapping[channel][output_state]);
		}
		// Wait for interrupt to change output state
		while(tmp_output_state == output_state){
			lcd_gotoxy(0,1);
			lcd_puts("Waiting");
		}

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
	for(int i=0;i<count;i++){
		tmp += ADC_Read(channel);
	}
	tmp /= count;
	return (unsigned char) tmp;
}


void TIMER_init( void ){
	// Initialisierung:

	TCCR1B |= (1<<CS11) | (1<<CS10);
	TCNT1 = 65411;
	TIMSK |= (1<<TOIE1);
	sei();
}

char* char_to_number(unsigned char c){
	static char* fixed = "000";

	unsigned char huns,tens,ones;

	huns = c / 100;
	c = c - huns*100;

	tens = c / 10;
	c = c - tens * 10;

	ones = c;

	fixed[0] = huns+'0';
	fixed[1] = tens+'0';
	fixed[2] = ones+'0';

	return fixed;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 65411;
	if(output_state == 15){
		output_state = 0;
		if(channel == 2){
			channel = 0;
		}
		else{
			channel++;
		}
	}
	else{
		output_state++;
	}

	PORTB = (PORTB&0xf0) | output_state;

	//test_char++;

}
