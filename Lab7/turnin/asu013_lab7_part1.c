/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise # Hello World
 *	Exercise Description: Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive 
 *	the LCD display, initially displaying 0. Pressing PA0 increments the display (stopping at 
 *	9). Pressing PA1 decrements the display (stopping at 0). If both buttons are depressed 
 *	(even if not initially simultaneously), the display resets to 0. If a button is held, 
 *	then the display continues to increment (or decrement) at a rate of once per second. Use 
 *	a synchronous state machine captured in C.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://drive.google.com/file/d/1LQyb98C5jmLamT9jExEw1r3x_H4zmeP4/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
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

enum State{Start, Count, Wait} State;

void Tick();

unsigned char counter = 0;
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */

	TimerSet(100);
        TimerOn();

	State = Start;
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
    while (1) {
	Tick();
	LCD_WriteData(counter + '0'); // will display # on the LCD
	while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}

void Tick(){
	LCD_ClearScreen();
	unsigned char button1 = ~PINA & 0x01;
	unsigned char button2 = ~PINA & 0x02;
	switch(State){
		case Start:
			State = Count;
			break;
		case Count:
			if(button1 && !button2 && counter != 9){ //increase
				counter++;
			}
			else if(button2 && !button1 && counter != 0){ //decrease
				counter--;
			}
			else if(button1 && button2){
				counter = 0;
			}
			else{
				State = Count;
			}
			State = Wait;
			break;
		case Wait:
			State = Count;
			break;
		default:
			State = Start;
			break;
	}
}

