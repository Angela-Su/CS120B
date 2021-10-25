/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise # 2
 *	Exercise Description: Extend the earlier light game to maintain a score on the LCD 
 *	display. The initial score is 5. Each time the user presses the button at the right time 
 *	(the middle LED), the score increments. Each time the user fails, the score decrements. 
 *	When reaching 9, show victory somehow. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: 
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

enum State{ Start, Light1, Light2, Light3, Press, Wait1, Wait2 } State;

void Tick();

unsigned char counter = 5;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Insert your solution below */
	TimerSet(300);
        TimerOn();
        
	State = Start;
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);

    	while (1) {
		Tick();
		LCD_WriteData(counter + '0');
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    return 1;
}

void Tick(){
	LCD_ClearScreen();
	unsigned char temp = ~PINA & 0x01;
        switch(State){
                case Start:
                        State = Light1;
                        break;
                case Light1:
                        if(temp){
				if(counter != 0){
					counter--;
				}
                                State = Wait1;
                        } 
			else {
                                State = Light2;
                        }
                        break;
                case Light2:
                        if(temp){
				if(counter == 9){
					LCD_DisplayString(1, "VICTORY!");
					counter = 5;
				}
				else{
					counter++;
				}
                                State = Wait1;
                        } 
			else {
                                State = Light3;
                        }
                        break;
                case Light3:
                        if(temp){
				if(counter != 0){
					counter--;
				}
                                State = Wait1;
                        } 
			else {
                                State = Light1;
                        }
                        break;
                case Wait1:
                        if(temp){
                                State = Wait1;
                        } 
			else {
                                State = Press;
                        }
                        break;
                case Press:
                        if(temp){
                                State = Wait2;
                        } 
			else {
                                State = Press;
                        }
                        break;
                case Wait2:
                        if(temp){
                                State = Wait2;
                        } 
			else {
                                State = Start;
                        }
                default:
                        State = Start;
                        break;

        }
        switch(State){
                case Start:
                        PORTB = 0x00;
                        break;
                case Light1:
                        PORTB = 0x01;
                        break;
                case Light2:
                        PORTB = 0x02;
                        break;
                case Light3:
                        PORTB = 0x04;
                        break;
                case Wait1:
                        break;
                case Wait2:
                        break;
                default:
                        break;
        }
}
