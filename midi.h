/*
 * midi.h
 *
 *  Created on: 07.01.2015
 *      Author: Bubble
 */

#ifndef MIDI_H_
#define MIDI_H_

#include <avr/io.h>
#include "lcd.h"

#define USART_BAUDRATE 31250
#define BAUD_PRESCALE (((XTAL/ (USART_BAUDRATE * 16UL))) - 1)

extern const char (*control_names[48])[16];


extern void initialize_midi_map( void );
extern void MIDI_init( void );
extern void midi_send( char s );

volatile char* mapping[3][16];

#endif /* MIDI_H_ */
