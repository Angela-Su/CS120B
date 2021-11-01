/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: Using the ATmega1284’s built in PWM functionality, design a system 
 *	where a short, five-second melody, is played when a button is pressed. NOTE: The melody 
 *	must be somewhat complex (scaling from C to B is NOT complex). Criteria:
 *	When the button is pressed, the melody should play until completion
 *	Pressing the button again in the middle of the melody should do nothing
 *	If the button is pressed and held, when the melody finishes, it should not repeat until 
 *	the button is released and pressed again
 *	Demo Link: https://drive.google.com/file/d/1N2M5xAW0iOZgXoKMedTdASMyLbsdQyNH/view?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

void TimerISR()
{
    TimerFlag = 1;
}

void TimerOff()
{
    TCCR1B = 0x00;
}

void TimerOn()
{
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;

}

ISR(TIMER1_COMPA_vect)
{
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0)
    {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

double melodies[24] = { 329.63, 349.23, 392, 493.88, 440, 392, 392, 0
                        440, 392, 349.23, 349.23, 0, 349.23, 0, 349.23, 0, 349.23, 0, 349.23
                        392, 349.23, 329.63, 329.63};
int counter = 0;

enum SM_STATES{ SM_Init, SM_Wait, SM_Hold, SM_Melody, SM_Done } SM_STATE;
void Tick_Func(){
        unsigned char temp = ~PINA & 0x01;
        switch(SM_STATE){
                case SM_Init:
                        SM_STATE = SM_Wait;
                        break;
                case SM_Wait:
                        if(temp == 0x01){
                                SM_STATE = SM_Melody;
                        } 
			else {
                                SM_STATE = SM_Wait;
                        }
                        break;
                case SM_Melody:
                        if(counter < 24){
                                SM_STATE = SM_Melody;
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
                case SM_Melody:
                        set_PWM(melodies[counter]);
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
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0x40; PORTB = 0x00;
    /* Insert your solution below */
        TimerSet(200);
        TimerOn();
        PWM_on();
        SM_STATE = SM_Init;
    while (1) {
        Tick_Func();
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
