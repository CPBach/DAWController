/*
 * main.c
 *
 *  Created on: 24.12.2014
 *      Author: stefan
 */



#include <avr/io.h>

#define RUNNING 1
#define SUCCESS 0

int main ( void ){
	while(RUNNING){
		PORTA |= 0xff;
	}

	return SUCCESS;
}
