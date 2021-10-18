/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description:  Create your own festive lights display with 6 LEDs connected to port PB5..PB0, 
 *	lighting in some attractive sequence. Pressing the button on PA0 changes the lights to the next 
 *	configuration in the sequence.  Use a state machine (not synchronous) captured in C. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo: https://www.tiktok.com/@confused.asian/video/7020237876759678213?is_copy_url=0&is_from_webapp=v1&lang=en&sender_device=pc&sender_web_id=7020238378064250374
 *            https://drive.google.com/file/d/1JSoT_ImsXj83ZpNKywg6JtKICD3MLaVw/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Init, Wait0, Press1, Wait1, Press2, Wait2, Press3, Wait3, Press4, Wait4} States;
void Tick();

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // output
	//DDRC = 0xFF; PORTC = 0x00; // output
	
	States = Start;
	while(1){
		Tick();
    	}

	return 0;
}

void Tick(){
	unsigned char temp = ~PINA & 0x01;
        switch(States){
                case Start:
                        States = Init;
                        break;
                case Init:
                        if(temp){
                                States = Wait0;
                        } else {
                                States = Init;
                        }
                        break;
                case Wait0:
                        if(temp){
                                States = Wait0;
                        } else {
                                States = Press1;
                        }
                        break;
                case Press1:
                        if(temp){
                                States = Wait1;
                        } else {
                                States = Press1;
                        }
                        break;
                case Wait1:
                        if(temp){
                                States = Wait1;
                        } else {
                                States = Press2;
                        }
                        break;
                case Press2:
                        if(temp){
                                States = Wait2;
                        } else {
                                States = Press2;
                        }
                        break;
                case Wait2:
                        if(temp){
                                States = Wait2;
                        } else {
                                States = Press3;
                        }
                        break;
                case Press3:
                        if(temp){
                                States = Wait3;
                        } else {
                                States = Press3;
                        }
                        break;
                case Wait3:
                        if(temp){
                                States = Wait3;
                        } else {
                                States = Press4;
                        }
                        break;
                case Press4:
                        if(temp){
                                States = Wait4;
                        } else {
                                States = Press4;
                        }
                        break;
                case Wait4:
                        if(temp){
                                States = Wait4;
                        } else {
                                States = Init;
                        }
                        break;
                default:
                        States = Init;
                        break;
        }
		switch(States){
                case Start:
                        PORTB = 0x00;
                        break;
                case Init:
                        break;
                case Wait0:
                        PORTB = 0x15;
                        break;
                case Press1:
                        break;
                case Wait1:
                        PORTB = 0x00;
                        break;
                case Press2:
                        break;
                case Wait2:
                        PORTB = 0x2A;
                        break;
                case Press3:
                        break;
                case Wait3:
                        PORTB = 0x00;
                        break;
                case Press4:
                        break;
                case Wait4:
                        PORTB = 0x3F;
                        break;
                default:
                        break;
        }
}
