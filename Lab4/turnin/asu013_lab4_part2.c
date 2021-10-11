/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 
 *	7. Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once 
 *	(stopping at 0). If both buttons are depressed (even if not initially simultaneously), 
 *	PORTC resets to 0.
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
	
	while(1){
		Tick();
	}

	return 0;
}

enum States{Start, Press, Inital, Reset, Plus, Minus, Plus_On, Minus_On} state;

void Tick(){
switch(state) {
	case Start:
		state = Press;
		break;
	case Press:
		state = Inital;
		break;
	case Initial:
		if ((PINA & 0x03) == 0x03) {
			state = Reset;
		}
		else if ((PINA & 0x01) == 0x01) {
			state = PlusOn;
		}
		else if ((PINA & 0x02) == 0x02) {
			state = MinusOn;
		}
		break;
	case Reset:
		if ((PINA & 0x03) == 0x03) {
			state = Reset;
		}
		else {
			state = Init;
		}
		break;
	case Plus:
		if ((PINA & 0x01) == 0x01) {
			state = Plus;
		}
		else {
			state = Init;
		}
		break;
	case Plus_On:
		state = Plus;
		break;	
	case Minus:
		if ((PINA & 0x02) == 0x02) {
			state = Minus;
		}
		else {
			state = Init;
		}
		break;
	case Minus_On:
		state = Minus;
		break;	
	default:
		state = Start;
		break;
}

switch(state) {
	case Start:
		PORTC = 0x07;
		break;
	case Press:
		PORTC = 0x07;
		break;
	case Initial:
	case Plus:
	case Minus:
		break;
	case Plus_On:
		if (PORTC < 0x09) {
                       	PORTC = PORTC + 1;
		}
                break;
	case Minus_On:
		if (PORTC > 0x00) { 
                        PORTC = PORTC - 1;
		}
                break;
	case Reset:
		PORTC = 0x00;
		break;
	default:
		PORTC = 0x07;
		break;
	}
}


