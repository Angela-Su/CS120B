/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #5
 *	Exercise Description: A car's passenger-seat weight sensor outputs a 9-bit value (ranging 
 *	from 0 to 511) and connects to input PD7..PD0PB0 on the microcontroller. If the weight is 
 *	equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. If the 
 *	weight is above 5 but below 70, the airbag should be disabled and an "Airbag disabled" 
 *	icon should light by setting PB2 to 1. (Neither B1 nor B2 should be set if the weight is 5 
 *	or less, as there is no passenger). 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	//DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x01; PORTB = 0x00; // this gives us 1111 1110
	DDRD = 0x00; PORTD = 0xFF; 

	unsigned short temp = 0x00; // used for shifting 
	unsigned char answer = 0x00; //used for outputting

while(1) {
	temp = (PIND << 1) | (PINB & 0x01);
	if(temp >= 70){
		PORTB = answer | 0x02;
	}
	else if((temp > 5)){
		PORTB = 0x04;
	}
	else{
		PORTB = 0x00;
	}
	temp = 0;
	answer = 0;
}
	return 0;
}


