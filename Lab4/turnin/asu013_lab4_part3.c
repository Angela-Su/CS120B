/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: A household has a digital combination deadbolt lock system on the 
 *	doorway. The system has buttons on a keypad. Button 'X' connects to PA0, 'Y' to PA1, and 
 *	'#' to PA2. Pressing and releasing '#', then pressing 'Y', should unlock the door by 
 *	setting PB0 to 1. Any other sequence fails to unlock. Pressing a button from inside the 
 *	house (PA7) locks the door (PB0=0).
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

enum States{Start, Inital, Unlock_Bolt, Unlock, Release, Locked} state;

void Tick(){
switch(state) {
	switch(state) {
		case Start:
			state = Initial;
			break;
		case Initial:
			if ((PINA & 0x07) == 0x04) {
				state = Unlock_Bolt;
			}
			else if ((PINA & 0x87) == 0x80) {
				state = Locked;
			}
			else {
				state = Initial;
			}
			break;
		case Unlock_Bolt:
			if ((PINA & 0x07) == 0x04) {
                                state = Unlock_Bolt;
                        }
                        else {
                                state = Unlock;
                        }
                        break;
		case Unlock:
			if ((PINA & 0x07) == 0x02) {
                                state = Release;
                        }
			else if (PINA == 0x00) {
				state = Unlock;
			}
                        else {
                                state = Initial;
                        }
                        break;
		case Release:
			if ((PINA & 0x07) == 0x02) {
                                state = Release;
                        }
                        else {
                                state = Initial;
                        }
                        break;
		case Locked:
			if ((PINA & 0x87) == 0x80) {
                                state = Locked;
                        }
                        else {
                                state = Initial;
                        }
                        break;
	}
	switch(state) {
		case Start:
			PORTB = 0x00;
			break;
		case Initial:
		case Unlock_Bolt:
		case Unlock:
			break;
		case Release:
			PORTB = 0x01;
			break;
		case Locked:
			PORTB = 0x00;
			break;

	}
}


