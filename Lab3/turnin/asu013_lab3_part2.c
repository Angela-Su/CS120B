/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: A car has a fuel-level sensor that sets PA3..PA0 to a value between 
 *	0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to 
 *	graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level 
 *	is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights 
 *	PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should 
 *	light if the level is 4 or less.  (The example below shows the display for a fuel level of 
 * 	3).   
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
	//DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // output

	unsigned char A = 0x00;
	//unsigned char B = 0x00;
	unsigned char C = 0x00;

while(1) {
	A = PINA & 0x0F;
	//B = PINB;
	C = 0x00;
	
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

	PORTC = C;
}
	return 0;
}


