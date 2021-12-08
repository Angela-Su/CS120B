/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Final Project
 *	Exercise Description: DDR
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "io.h"
#include "bit.h"

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency){
            TCCR3B &= 0x08;
	}
        else{
            TCCR3B |= 0x03;
	}
        if(frequency < 0.954){
            OCR3A = 0xFFFF;
	}
        else if (frequency > 31250){
            OCR3A = 0x0000;
	}
        else{
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
	}
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

volatile unsigned char TimerFlag = 0; 
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80; 
}

void TimerOff() {
	TCCR1B = 0x00; 
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned short ControllerIn(){ //controller's inputs
	unsigned short data = 0x0000;
	PORTA = SetBit(PINA, 1, 1);
	    _delay_us(12);
	PORTA = SetBit(PINA, 1, 0);
	    _delay_us(6);

	for (unsigned char i = 0; i < 12; i++){
		if(!GetBit(PINA, 2)){
			data |= (1 << i);
		}
		else{
			data &= ~(1 << i);
		}
		
		PORTA = SetBit(PINA, 0, 1);
		    _delay_us(10);
		PORTA = SetBit(PINA, 0, 0);
		    _delay_us(10);
	}
	return data;
}

char* str = "";
unsigned char Cinputs = 0; //controller inputs
unsigned char Aoutputs = 0; //arduino outputs
int buttonNum = 0;
enum buttonSM{BButton, YButton, Select, Start, Up, Down, Left, Right, AButton, XButton, LButton, RButton, Init} buttons;
void control(){
	
	switch(buttons){
		case Init:
            if(GetBit(ControllerIn(), 0)){
                buttons = BButton;
            }
            else if(GetBit(ControllerIn(), 1)){
                buttons = YButton;
            }
            else if(GetBit(ControllerIn(), 2)){
                buttons = Select;
            }
            else if(GetBit(ControllerIn(), 3)){
                buttons = Start;
            }
            else if(GetBit(ControllerIn(), 4)){
                buttons = Up;
            }
            else if(GetBit(ControllerIn(), 5)){
                buttons = Down;
            }
            else if(GetBit(ControllerIn(), 6)){
                buttons = Left;
            }
            else if(GetBit(ControllerIn(), 7)){
                buttons = Right;
            }
            else if(GetBit(ControllerIn(), 8)){
                buttons = AButton;
            }
            else if(GetBit(ControllerIn(), 9)){
                buttons = XButton;
            }
            else if(GetBit(ControllerIn(), 10)){
                buttons = LButton;
            }
            else if(GetBit(ControllerIn(), 11)){
                buttons = RButton;
            }
            else{
                buttons = Init;
            }
            break;

		case BButton: 
		case YButton: 
		case Select: 
		case Start: 
		case Up: 
		case Down: 
		case Left: 
		case Right: 
		case AButton: 
		case XButton: 
		case LButton: 
		case RButton: 
            buttons = Init; 
            break;
		default:
			buttons = BButton;
			break;
	}
	
	switch(buttons){
		case Init: 
            LCD_DisplayStr(1, str); 
            Cinputs = 0x00; break;
		case BButton: 
            Aoutputs = 0x01;
            str = "B"; 
            Cinputs = 0x10; break;
		case YButton: 
            str = "Y"; 
            Cinputs = 0x20; break;
		case Select:  
            str = "SELECT"; 
            Cinputs = 0x30; break;			
		case Start:	
            str = "START"; 
            Cinputs = 0x40; break;
		case Up:
            str = "UP"; 
            Cinputs = 0x50; break;
		case Down:	
            str = "DOWN";
            Cinputs = 0x60; break;
		case Left:	
            str = "LEFT"; 
            Cinputs = 0x70; break;
		case Right:	
            str = "RIGHT";
            Cinputs = 0x80; break;
		case AButton:
            Aoutputs = 0x02;
            str = "A"; 
            Cinputs = 0x90; break;
		case XButton:
            str = "X"; 
            Aoutputs = 0x03;
            Cinputs = 0xA0; break;
		case LButton:
            str = "L"; 
            Cinputs = 0xB0; break;
		case RButton:
            str = "R"; 
            Cinputs = 0xC0; break;
        default:
            str = "break";
            Cinputs = 0x00;
            break;
	}
};

double songEZ[27] = { 329.63, 329.63, 349.23, 349.23, 392, 392, 493.88, 440, 392, 392, 0,
                        440, 392, 349.23, 349.23, 0, 349.23, 0, 349.23, 0, 349.23, 0, 349.23,
                        392, 349.23, 329.63, 329.63};
double songMid[27] = { 329.63, 329.63, 349.23, 349.23, 392, 392, 493.88, 440, 392, 392, 0,
                        440, 392, 349.23, 349.23, 0, 349.23, 0, 349.23, 0, 349.23, 0, 349.23,
                        392, 349.23, 329.63, 329.63};
double songHard[27] = { 329.63, 329.63, 349.23, 349.23, 392, 392, 493.88, 440, 392, 392, 0,
                        440, 392, 349.23, 349.23, 0, 349.23, 0, 349.23, 0, 349.23, 0, 349.23,
                        392, 349.23, 329.63, 329.63};
int counter = 0;

enum SM_STATES{ SM_Init, SM_Wait, SM_Hold, Song1, Song2, Song3, SM_Done } SM_STATE;
void Tick_Func(){
        unsigned char temp = PINB; //0x02 0x04 0x08
        switch(SM_STATE){
                case SM_Init:
                        SM_STATE = SM_Wait;
                        break;
                case SM_Wait:
                        if(temp == 0x02){
                                SM_STATE = Song1;
                        }
                        else if(temp == 0x04){
                                SM_STATE = Song2;
                        }
                        else if(temp == 0x08){
                                SM_STATE = Song3;
                        }  
			else {
                                SM_STATE = SM_Wait;
                        }
                        break;
                case Song1:
                        if(counter < 27){
                                SM_STATE = Song1;
                        } 
			else {
                                SM_STATE = SM_Done;
                        }
                        break;
                case Song2:
                        if(counter < 27){
                                SM_STATE = Song2;
                        } 
			else {
                                SM_STATE = SM_Done;
                        }
                        break;
                case Song3:
                        if(counter < 27){
                                SM_STATE = Song3;
                        } 
			else {
                                SM_STATE = SM_Done;
                        }
                        break;
                case SM_Done:
                        SM_STATE = SM_Hold;
                        break;
                case SM_Hold:
                        if(temp == 0x01){
                                SM_STATE = SM_Done;
                        } 
			else {
                                SM_STATE = SM_Wait;
                        }
                        break;
                default:
                        break;
        }
        switch(SM_STATE){
                case SM_Init:
                        set_PWM(0);
                        break;
                case SM_Wait:
                        break;
                case Song1:
                        set_PWM(songEZ[counter]);
                        counter++;
                        break;
                case Song2:
                        set_PWM(songMid[counter]);
                        counter++;
                        break;
                case Song3:
                        set_PWM(songHard[counter]);
                        counter++;
                        break;
                case SM_Done:
                        set_PWM(0);
                        counter = 0;
                        break;
                case SM_Hold:
                        break;
                default:
                        break;
        }


}

int main(void) {
	DDRA = 0x03; 
	DDRB = 0x0F; PORTB = 0xF0; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x10; PORTD = 0x00;
	
	TimerSet(2);
	TimerOn();

    PWM_on();
    SM_STATE = SM_Init;
	
	LCD_init();
	LCD_ClearScreen();
	
	while(1) {
		control();
        Tick_Func();
        PORTC = Aoutputs;
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}