/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: PB0 and PB1 each connect to an LED, and PB0's LED is initially on. 
 *	Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying 
 *	that way after button release. Pressing the button again turns off PB1's LED and turns on 
 *	PB0's LED. 
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
	DDRB = 0xFF; PORTB = 0x00; // output
	//DDRC = 0xFF; PORTC = 0x00; // output
	
	while(1){
		Tick();
	}

	return 0;
}

enum States{Start, On_Press, On_Release, Off_Press, Off_Release} state;

void Tick(){
switch(state) {
	case Start:
		state = On_Press;
		break;
	case On_Press:
		if ((PINA & 0x01) == 0x01) {
			state = On_Press;
		}
		else {
			state = On_Release;
		}
		break;
	case On_Release:
		if ((PINA & 0x01) == 0x01) {
			state = Off_Press;
		}
		else {
			state = On_Release;
		}
		break;
	case Off_Press:
		if ((PINA & 0x01) == 0x01) {
			state = Off_Press;
		}
		else {
			state = Off_Release;
		}
		break;
	case Off_Release:
		if ((PINA & 0x01) == 0x01) {
			state = On_Press;
		}
		else {
			state = Off_Release;
		}
		break;
	default:
		state = Start;
		break;
}
switch(state) {
	case Start:
		PORTB = 0x01;
		break;
	case On_Press:
		PORTB = 0x01;
		break;
	case On_Release:
		break;
	case Off_Press:
		PORTB = 0x02;
		break;
	case Off_Release:
		break;
	default:
		PORTB = 0x01;
		break;
}
}


