/*
DAW Controller - AVR-Software for controlling Cubase/Logic/...
Copyright (C) 2014 Stefan Babel

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
		 * to 0. When going back to 0 we increment the channel number.
		 * (ADC0 - ADC2)
		 * This is done to control the 4067 multiplexer IC's.
		 * In our configuration we have 3 multiplexer ICs all being
		 * controlled over PORTB ( The least significant 4 bits )I
		 * IC1 is on ADC0, IC2 is on ADC1, IC3 is on ADC2.
		 *
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
			// TODO: Implement the right midi messages
			midi_send(midi_messages[channel][output_state][0]);
			midi_send(midi_messages[channel][output_state][1]);
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

