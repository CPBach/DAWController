/*
 * main.c
 *
 *  Created on: 24.12.2014
 *      Author: stefan
 */



#include <avr/io.h>
#include<avr/sleep.h>

// Maybe implement our own abs function, instead of importing the whole stdlib
#include <stdlib.h>

#include "lcd.h"
#include "midi.h"
#include "adc.h"
#include "interrupt.h"
#include "helper.h"

#include "main.h"


volatile unsigned char output_state = 0x00;
volatile unsigned char channel = 0x00;

volatile unsigned char values[3][16];


int main ( void ){

	char* headtitle = "DAWController1.0";

	// Initialize Display
	lcd_init(LCD_DISP_ON);

	// Display title
	lcd_puts(headtitle);

	// PORTA are all inputs
	DDRA = 0x00;
	PORTA = 0x00;
	// Define the rest as outputs
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;

	// Initialize all units
	ADC_Init();
	MIDI_init();
	TIMER_init();

	// Run this loop forever
	while(RUNNING){
		/*
		 * The ISR increments output_state from 0 to 15 and then back
		 * to 0. This is done to control the 4067 multiplexer IC's.
		 * So we discretize the voltage at the current channel
		 * and then wait for the outputstate to be changed by the
		 * ISR. See the implementation in interrupt.c.
		 */
		unsigned char tmp_output_state = output_state;

		// We average the reading to avoid reading inaccuracies.
		unsigned char reading = avg_metering(channel, 16);

		// TODO: Think of another way to avoid ripples
		if (abs(((int16_t)values[channel][output_state]) - reading) >1){
			values[channel][output_state] = reading;
			// TODO: Implement midi messages
			midi_send(midi_messages[0][0][0]);
			midi_send(midi_messages[0][0][1]);
			midi_send(reading);
			lcd_gotoxy(0,1);
			lcd_puts(mapping[channel][output_state]);
			lcd_puts(char_to_number(reading));
		}

		// Wait for interrupt to change output state
		while(tmp_output_state == output_state){
		}
	}
	// We should never reach this line...
	return SUCCESS;
}

