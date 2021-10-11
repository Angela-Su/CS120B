/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: Count the number of 1s on ports A and B and output that number on
 *	port C. 
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
	A = PINA;
	B = PINB;
	C = 0x00;
	
	while(A != 0x00){
		if(A & 0x01){
			C = C + 1;
		}
		A = A >> 1;
	}
	while(B != 0x00){
		if(B & 0x01){
			C = C + 1;
		}
		B = B >> 1;
	}
	PORTC = C;
}
	return 0;
}


