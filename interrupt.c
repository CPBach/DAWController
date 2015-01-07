#include <avr/interrupt.h>
#include "main.h"

void TIMER_init( void ){
	TCCR1B |= (1<<CS11) | (1<<CS10);
	TCNT1 = 65411;
	TIMSK |= (1<<TOIE1);
	sei();
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
}
