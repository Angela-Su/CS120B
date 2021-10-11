/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #3
 *	Exercise Description: In addition to the above, PA4 is 1 if a key is in the ignition, PA5 
 *	is 1 if a driver is seated, and PA6 is 1 if the driver's seatbelt is fastened. PC7 
 *	should light a "Fasten seatbelt" icon if a key is in the ignition, the driver is seated, 
 *	but the belt is not fastened.   
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // output

	unsigned char A = 0x00;
	unsigned char B = 0x00;
	unsigned char C = 0x00;

while(1) {
	A = PINA & 0x0F;
	B = PINB;
	C = 0x40;
	
	switch(A){
		case 1 :
		case 2 :
			C = 0x20;
			C = C | 0x40;
			break;
		case 3 :
		case 4 :
			C = 0x30;
			C = C | 0x40;
			break;
		case 5 :
		case 6 :
			C = 0x38;
			break;
		case 7 :
		case 8 :
		case 9 :
			C = 0x3C;
			break;
		case 10 :
		case 11 :
		case 12 :
			C = 0x3E;
			break;
		case 13 :
		case 14 :
		case 15 :
			C = 0x3F;
			break;
	}
	B = PINA & 0xF0;
	if(B == 0x30){
		C = C |0x80;
	}

	PORTC = C;
}
	return 0;
}


