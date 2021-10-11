/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #4
 *	Exercise Description: Read an 8-bit value on PA7..PA0 and write that value on 
 *	PB3..PB0PC7..PC4. That is to say,  take the upper nibble of PINA and map it to the lower 
 *	nibble of PORTB, likewise take the lower nibble of PINA and map it to the upper nibble of 
 *	PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
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
	DDRB = 0xFF; PORTB = 0x00; // output1
	DDRC = 0xFF; PORTC = 0x00; // output2

	unsigned char A = 0x00; //temp values for outputting
	unsigned char B = 0x00;

while(1) {
	A = (PINA & 0xFF) >> 4;
	B = (PINA & 0xFF) << 4;
	
	PORTB = A;
	PORTC = B;
}
	return 0;
}


