#include "midi.h"

const unsigned char (*control_names[48])[16] =
{"Vol.Ch 1:    ","Vol.Ch 2:    ","Vol.Ch 3:    ","Vol.Ch 4:    ","Vol.Ch 5:    ","Vol.Ch 6:    ","Vol.Ch 7:    ","Vol.Ch 8:    ",
 "GP1 8:       ","GP1 7:       ","GP1 6:       ","GP1 5:       ","GP1 4:       ","GP1 3:       ","GP1 2:       ","GP1 1:       ",
 "DELAY:       ","REPEAT:      ","MON LEVEL:   ","LEVEL:       ","MstSldLvl L: ","MstSldLvl R: ","ECHOBtn:     ","MO Btn:      ",
 "GP2 8:       ","GP2 7:       ","GP2 6:       ","GP2 5:       ","GP2 4:       ","GP2 3:       ","GP2 2:       ","GP2 1:       ",
 "GP4 1:       ","GP4 2:       ","GP4 3:       ","GP4 4:       ","GP4 5:       ","GP4 6:       ","GP4 7:       ","GP4 8:       ",
 "GP3 8:       ","GP3 7:       ","GP3 6:       ","GP3 5:       ","GP3 4:       ","GP3 3:       ","GP3 2:       ","GP3 1:       "};



void MIDI_init(void) {
	// 31,250 baud, asynchronous, 1 start bit, 1 stop bit
	// Use 8-bit character sizes, 1 start,
	// 1 stop bit - URSEL bit set to select the UCRSC register
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;
	// Turn on the transmission circuitry
	UCSRB = (1 << TXEN);

	initialize_midi_map();
}

void midi_send(char s) {
	/* Wait for empty transmit buffer */
	while (!( UCSRA & (1 << UDRE)))
		;
	UDR = s;
}

void initialize_midi_map(void) {
	/*
	 * Maps to control_names
	 */
	for (int i = 0; i < 16; i++) {
		mapping[0][i] = control_names[i];
		mapping[1][i] = control_names[i + 16];
		mapping[2][i] = control_names[i + 32];
	}

	for (int i = 0; i < 8; i++) {
		// Initialize volume sliders
		midi_messages[0][i][0] = CONTROL_CHANGE_CH1 + i;
		midi_messages[0][i][1] = VOLUME;

		// Initialize GP1 controllers
		midi_messages[0][8 + i][0] = CONTROL_CHANGE_CH1 + 7 - i;
		midi_messages[0][8 + i][1] = GENERAL_PURPOSE_CONTROLLER1;

		// Initialize GP4 & GP3 controllers
		midi_messages[2][i][0] = CONTROL_CHANGE_CH1 + i;
		midi_messages[2][i][1] = GENERAL_PURPOSE_CONTROLLER1 + 3;

		midi_messages[2][8 + i][0] = CONTROL_CHANGE_CH1 + 7 - i;
		midi_messages[2][8 + i][1] = GENERAL_PURPOSE_CONTROLLER1 + 2;

		// Initialize some stuff to GP 5
		midi_messages[1][i][0] = CONTROL_CHANGE_CH1 + i;
		midi_messages[1][i][1] = GENERAL_PURPOSE_CONTROLLER5;

		// This goes to GP2
		midi_messages[1][8 + i][0] = CONTROL_CHANGE_CH1 + 7 - i;
		midi_messages[1][8 + i][1] = GENERAL_PURPOSE_CONTROLLER1 + 1;

	}

}

