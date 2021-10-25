/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 1
 *	Exercise Description: Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 
 *	in sequence, 1 second each. Implement that synchSM in C using the method defined in 
 *	class. In addition to demoing your program, you will need to show that your code adheres 
 *	entirely to the method with no variations.

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1L6VCVHFr8IXvl81cKKW7TPJl6MVrjCBY/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
        TCCR1B = 0x0B;
        OCR1A = 125;
        TIMSK1 = 0x02;

        TCNT1 = 0;

        _avr_timer_cntcurr = _avr_timer_M;

        SREG |= 0x80;
}

void TimerOff(){
        TCCR1B = 0x00;
}

void TimerISR(){
        TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
        _avr_timer_cntcurr--;
        if(_avr_timer_cntcurr == 0){
                TimerISR();
                _avr_timer_cntcurr = _avr_timer_M;
        }
}

void TimerSet(unsigned long M){
        _avr_timer_M = M;
        _avr_timer_cntcurr = _avr_timer_M;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Insert your solution below */
	TimerSet(1000);
        TimerOn();
        unsigned char temp = 0x00;

    	while (1) {
		temp = 0x01;
		PORTB = temp;
		while(!TimerFlag);
		TimerFlag = 0;
		temp = 0x02;
		PORTB = temp;
		while(!TimerFlag);
		TimerFlag = 0;
		temp = 0x04;
		PORTB = temp;
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    return 1;
}
