/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab 6  Exercise 3
 *	Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTB is initially 
 *	7. Pressing PA0 increments PORTB once (stopping at 9). Pressing PA1 decrements PORTB once 
 *	(stopping at 0). If both buttons are depressed (even if not initially simultaneously), 
 *	PORTB resets to 0. Now that we have timing, only check to see if a button has been 
 *	pressed every 100 ms. Additionally, if a button is held, then the count should continue 
 *	to increment (or decrement) at a rate of once per second.

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1LI1qZxopsahyDgjXB-Sn2ql6AxnZ6d0r/view?usp=sharing
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

unsigned char counter = 0x00;
unsigned char counter2 = 0x00;
enum State{ Init, Init2, Start, Increase, Decrease, Reset } State;

void Tick();

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Insert your solution below */
	TimerSet(100);
        TimerOn();
        
	State = Start;

    	while (1) {
		Tick();
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    return 1;
}

void Tick(){
	unsigned char temp = ~PINA & 0x03;
        switch(State){
                case Init:
                        State = Init2;
                        break;
                case Init2:
                        State = Start;
                        break;
                case Start:
                        counter2 = 0x00;
                        if(temp == 0x01){
                                if(counter < 0x09){
                                        counter++;
                                }
                                State = Increase;
                        } else if(temp == 0x02){
                                if(counter > 0x00){
                                        counter--;
                                }
                                State = Decrease;
                        } else if(temp == 0x03){
                                State = Reset;
                        } else {
                                State = Start;
                        }
                        break;
                case Increase:
                        if(temp == 0x01){
                                counter2++;
                                State = Increase;
                        } else if(temp == 0x03){
                                counter = 0x00;
                                State = Reset;
                        } else {
                                State = Start;
                        }
                        break;
                case Decrease:
                        if(temp == 0x02){
                                counter2++;
                                State = Decrease;
                        } else if(temp == 0x03){
                                counter = 0x00;
                                State = Reset;
                        } else {
                                State = Start;
                        }
                        break;
                case Reset:
                        if(temp == 0x03){
                                State = Reset;
                        } else if(temp == 0x01){
                                State = Increase;
                        } else if(temp == 0x02){
                                State = Decrease;
                        } else {
                                State = Start;
                        }
                        break;
                default:
                        State = Init;
                        break;

        }
        switch(State){
                case Init:
                        break;
                case Init2:
                        counter = 0x07;
                        PORTB = 0x07;
                        break;
                case Start:
                        PORTB = counter;
                        break;
                case Increase:
                        if(counter2 == 0x0A && counter < 0x09){
                                counter++;
                                counter2 = 0x00;
                        }
                        PORTB = counter;
                        break;
                case Decrease:
                        if(counter2 == 0x0A && counter > 0x00){
                                counter--;
                                counter2 = 0x00;
                        }
                        PORTB = counter;
                        break;
                case Reset:
                        counter = 0x00;
                        PORTB = 0x00;
                default:
                        break;
        }
}
