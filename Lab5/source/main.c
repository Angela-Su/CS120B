/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description:  Buttons are connected to PA0 and PA1. Output for PORTC is initially 
 *	7. Pressing PA0 increments PORTC (stopping at 9). Pressing PA1 decrements PORTC (stopping 
 *	at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets 
 *	to 0. If a reset occurs, both buttons should be fully released before additional 
 *	increments or decrements are allowed to happen. Use LEDs (and resistors) on PORTC. Use a 
 *	state machine (not synchronous) captured in C. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char iterator = 0x00;
enum States{Start, Init, StartProg, Increase, Decrease, Reset} States;
void Tick();

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	//DDRB = 0xFF; PORTB = 0x00; // output
	DDRC = 0xFF; PORTC = 0x00; // output

	unsigned char tempA = 0x00;
	unsigned char result = 0x00;
	
	States = Start;
	while(1){
		Tick();
    	}

	return 0;
}

void Tick(){
	unsigned char temp = ~PINA & 0x03;
        switch(States){
                case Start:
                        States = Init;
                        break;
                case Init:
                        States = StartProg;
                        break;
                case StartProg:
                        if(temp == 0x01){
                                if(iterator < 0x09){
                                        iterator++;
                                }
								States = Increase;
                        } 
						else if(temp == 0x02){
                                if(iterator > 0x00){
                                        iterator--;
                                }
                                States = Decrease;
                        } else if(temp == 0x03){
                                States = Reset;
                        } else {
                                States = StartProg;
                        }
                        break;
                case Increase:
                        if(temp == 0x01){
                                States = Increase;
                        } else if(temp == 0x03){
                                States = Reset;
                        } else {
                                States = StartProg;
                        }
                        break;
                case Decrease:
                        if(temp == 0x02){
                                States = Decrease;
                        } else if(temp == 0x03){
                                counter = 0x00;
                                States = Reset;
                        } else {
                                States = StartProg;
                        }
                        break;
                case Reset:
                        if(temp == 0x03){
                                States = Reset;
                        } else if(temp == 0x01){
                                States = Increase;
                        } else if(temp == 0x02){
                                States = Decrease;
                        } else {
                                States = StartProg;
                        }
                        break;
                default:
                        States = Init;
                        break;
        }
        switch (States){
                case Start:
                        break;
		case Init:
			iterator = 0x07;
                        PORTC = 0x07;
			break;
                case StartProg:
			PORTC = iterator;
			if(temp == 0x01){
				PORTC += 1;
			}
                        break;
                case Increase:
                        PORTC = iterator;
                        break;
                case Decrease:
                        PORTC = iterator;
                        break;
                case Reset:
                        iterator = 0x00;
                        PORTC = 0x00;
                        break;
                default:
                        break;
        }
}
