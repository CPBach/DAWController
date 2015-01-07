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


#define CONTROL_CHANGE_CH1 0xB0
#define GENERAL_PURPOSE_CONTROLLER1 0x10
#define VOLUME 0x07

extern const unsigned char (*control_names[48])[16];


extern void initialize_midi_map( void );
extern void MIDI_init( void );
extern void midi_send( char s );

char* mapping[3][16];
unsigned char midi_messages[3][16][2];

#endif /* MIDI_H_ */
