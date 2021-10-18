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
	
		State = Init;
	while(1){
		Tick();
    	}

	return 0;
}

void Tick(){
	unsigned char temp = ~PINA & 0x03;
        switch(State){
                case Init:
                        State = SM_Init2;
                        break;
                case SM_Init2:
                        State = SM_Start;
                        break;
                case SM_Start:
                        if(temp == 0x01){
                                if(counter < 0x09){
                                        counter++;
                                }
				State = SM_Inc;
                        } else if(temp == 0x02){
                                if(counter > 0x00){
                                        counter--;
                                }
                                State = SM_Dec;
                        } else if(temp == 0x03){
                                State = SM_Reset;
                        } else {
                                State = SM_Start;
                        }
                        break;
                case SM_Inc:
                        if(temp == 0x01){
                                State = SM_Inc;
                        } else if(temp == 0x03){
                                State = SM_Reset;
                        } else {
                                State = SM_Start;
                        }
                        break;
                case SM_Dec:
                        if(temp == 0x02){
                                State = SM_Dec;
                        } else if(temp == 0x03){
                                counter = 0x00;
                                State = SM_Reset;
                        } else {
                                State = SM_Start;
                        }
                        break;
                case SM_Reset:
                        if(temp == 0x03){
                                State = SM_Reset;
                        } else if(temp == 0x01){
                                State = SM_Inc;
                        } else if(temp == 0x02){
                                State = SM_Dec;
                        } else {
                                State = SM_Start;
                        }
                        break;
                default:
                        State = SM_Init2;
                        break;
        }
        switch (SM_STATE){
                case SM_Init:
                        break;
		case SM_Init2:
			counter = 0x07;
                        PORTC = 0x07;
			break;
                case SM_Start:
			PORTC = counter;
			if(temp == 0x01){
				PORTC += 1;
			}
                        break;
                case SM_Inc:
                        PORTC = counter;
                        break;
                case SM_Dec:
                        PORTC = counter;
                        break;
                case SM_Reset:
                        counter = 0x00;
                        PORTC = 0x00;
                        break;
                default:
                        break;
        }
}
