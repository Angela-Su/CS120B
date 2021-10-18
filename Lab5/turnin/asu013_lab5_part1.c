/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description:  A car has a fuel-level sensor that sets PA3..PA0 to a value between 
 *	0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to 
 *	graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level 
 *	is 3 or 4, PC5 and PC4 light. 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights 
 *	PC5..C1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should 
 *	light if the level is 4 or less. Use buttons on PA3..PA0 and mimic the fuel-level sensor 
 *	with presses.
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
	//DDRB = 0xFF; PORTB = 0x00; // output
	DDRC = 0xFF; PORTC = 0x00; // output

	unsigned char tempA = 0x00;
	unsigned char result = 0x00;
	
	while(1){
		result = 0x00;
		tempA = ~PINA & 0x0F; // add ~ so the microcontroller reads it
        	switch(tempA){ // check all the cases
			case 0x00:
				result = result | 0x40;
				break;
		        case 0x01:
		        case 0x02:
		                result = result | 0x60;
		                break;
		        case 0x03:
		        case 0x04:
		                result = result | 0x70;
		                break;
		        case 0x05:
		        case 0x06:
		                result = result | 0x38;
		                break;
		        case 0x07:
		        case 0x08:
		        case 0x09:
		                result = result | 0x3C;
		                break;
		        case 0x0A:
		        case 0x0B:
		        case 0x0C:
		                result = result | 0x3E;
		                break;
		        case 0x0D:
		        case 0x0E:
		        case 0x0F:
		                result = result | 0x3F;
		                break;
		        default:
		                result = result | 0x00;
		                break;
        	}
        	PORTC = result;
    	}

	return 0;
}


