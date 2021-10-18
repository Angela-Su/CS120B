/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Extra Credit HW1
 *	Exercise Description: I want to create a simple security system that checks if a house 
 *	with 4 windows is left open A3-0. If the window is open, sound the alarm related to the 
 *	window (meaning A0 corresponds to B0), otherwise, no alarm (have B3-0 be the alarm). Once 
 *	the alarm is sound, it must be turned off and the system is reset only when all windows 
 *	are closed. Make sure to set B to 0 when the windows are closed.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Window, Alarm, OffAlarm} States;
void Tick();

int main(void) {
    	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;

	States = Start;
    while (1) {
	Tick();
    }
    return 1;
}

void Tick(){
	switch(States){
		case Start:
			States = Window;
			PORTB = 0;
			break;
		case Window:
			if(PINA){
				States = Alarm;
			}
			break;
		case Alarm:
			States = OffAlarm;
			break;
		case OffAlarm:
			if(!PINA){
				States = Window;
			}
			break;
		default:
			States = Start;
			break;
	}
	switch(States){
		case Start:
			PORTB = 0;
			break;
		case Window:
			PORTB = 0;
			break;
		case Alarm:
			if(PINA & 0x01) PORTB = PORTB | 0x01;
			if(PINA & 0x02) PORTB = PORTB | 0x02;
			if(PINA & 0x04) PORTB = PORTB | 0x04;
			if(PINA & 0x08) PORTB = PORTB | 0x08;
			break;
		case OffAlarm:
			PORTB = 0;
			break;
		default:
			break;
	}
}
