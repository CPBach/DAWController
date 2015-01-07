/*
 * main.h
 *
 *  Created on: 07.01.2015
 *      Author: Bubble
 */

#include "lcd.h"

#ifndef MAIN_H_
#define MAIN_H_

#define RUNNING 1
#define SUCCESS 0

#define MULTIPLEXED_INPUTS 0x07
#define MULTIPLEXED_PORT PORTA

#define CONTROL_PORT PORTB

#ifdef LCD_PORT
#define DISPLAY_PORT LCD_PORT
#endif

volatile unsigned char output_state;
volatile unsigned char channel;


#endif /* MAIN_H_ */
