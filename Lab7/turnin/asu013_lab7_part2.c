/*	Author: Angela 
 *  Partner(s) Name: 
 *	Lab Section: 022
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

enum States{Start, Init, NextLed, Pause, Restart} state;

void Tick();

unsigned char counter = 5;
unsigned char count2 = 0;

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Insert your solution below */
	TimerSet(300);
        TimerOn();
        
	state = Start;
	LCD_init();

    	while (1) {
		Tick();
		while(!TimerFlag){};
		TimerFlag = 0;
    	}
    return 1;
}

void Tick(){
	LCD_ClearScreen();
	unsigned char temp = ~PINA & 0x01;
        if (counter == 0x00) {
		LCD_DisplayString(1, "0");
	}
	else if (counter == 0x01) {
		LCD_DisplayString(1, "1");
	}
	else if (counter == 0x02) {
                LCD_DisplayString(1, "2");
        }
	else if (counter == 0x03) {
                LCD_DisplayString(1, "3");
        }
	else if (counter == 0x04) {
                LCD_DisplayString(1, "4");
        }
	else if (counter == 0x05) {
                LCD_DisplayString(1, "5");
        }
	else if (counter == 0x06) {
                LCD_DisplayString(1, "6");
        }
	else if (counter == 0x07) {
                LCD_DisplayString(1, "7");
        }
	else if (counter == 0x08) {
                LCD_DisplayString(1, "8");
        }
	else if (counter == 0x09) {
                LCD_DisplayString(1, "9");
		LCD_DisplayString(2, "VICTORY!!!");
        }

	switch(state) {
		case Start:
			state = Init; 
			break;
		case Init:
			state = NextLed; 
			break;
		case NextLed:
			if (temp == 0x01) {
				state = Pause;
			}
			else {
				state = NextLed;
			}
			break;
		case Pause:
			if (temp == 0x01) {
				state = Pause;
			}
			else {
				state = Restart;
			}
			break;
		case Restart:
			if (temp == 0x01) {
				state = Init;
			}
			else {
				state = Restart;
			}
			break;
		default:
			state = Start; 
			break;
	}
	switch (state) {
		case Start:     
			counter = 0x05;
			break;
                case Init:      
			PORTB = 0x01; 
			break;
		case NextLed:
			if (count2 == 0x00) {
				if (PORTB == 0x04) {
					PORTB = PORTB >> 1;
					count2 = 0x01;
				}
				else {
					PORTB = PORTB << 1;
				}
			}
			else {
				if (PORTB == 0x01) {
                                        PORTB = PORTB << 1;
                                        count2 = 0x00;
                                }
                                else {
                                        PORTB = PORTB >> 1;
                                }
			}
			break;
		case Pause: 	
			if (PORTB == 0x02) {
				++counter;
			}
			else {
				--counter;
			} 
			break;
		case Restart:	
                default:        
			break;
	}
}