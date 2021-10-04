/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: Challenge : An amusement park kid ride cart has three seats, with 8-
 *	bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). Set 
 *	PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. Also, the  
 * 	cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg.  
 * 	The remaining 6 bits on D should display an approximation of the total combined weight.
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
	DDRB = 0x00; PORTB = 0xFF; // set everything to A; open ports for input
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00; //output

	unsigned char A = 0x00;
	unsigned char B = 0x00;
	unsigned char C = 0x00;
	unsigned char tempD = 0x00;

while(1) {
	A = PINA;
	B = PINB;
	C = PINC; 
	tempD = 0x00;

	if((A + B + C) > 0x8C ){ //check total weight doesnt exceed 140 = 0x8C
		tempD = 0x01;
	}
	if(((A-C)|(C-A)) > 0x50){ // check that it's not difference of 80+
		tempD = tempD + 0x02;
	}
	unsigned char tempTotal = 0x00;
	tempTotal = A+B+C & 0xFC;
	tempTotal = tempD + tempTotal;

	PORTD = tempTotal;
}
	return 0;
}


