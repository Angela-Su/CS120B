/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise 3
 *	Exercise Description: To the previous exercise's implementation, connect your speaker's 
 *	red wire to PB4 and black wire to ground. Add a third task that toggles PB4 on for 2 ms 
 *	and off for 2 ms as long as a switch on PA2 is in the on position. Don’t use the PWM for 
 *	this task.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

unsigned char threeLEDs = 0x00;
int counter = 0;
enum ThreeStates{Init, ST1, ST2, ST3, Restart} ThreeStates;
void ThreeLEDsSM(){
        switch(ThreeStates){
                case ST1:
                        ThreeStates = ST2;
                        break;
                case ST2:
                        ThreeStates = ST3;
                        break;
                case ST3:
                        ThreeStates = ST1;
                        break;
                default:
                        ThreeStates = ST1;
                        break;
        }
        switch(ThreeStates){
                case ST1:
                        threeLEDs = 0x01;
                        break;
                case ST2:
                        threeLEDs = 0x02;
                        break;
                case ST3:
                        threeLEDs = 0x04;
                        break;
                case Restart:
                       counter = 0;
                        break;
                default:
                        break;
        }

}

int time = 0;
unsigned char blinkingLED = 0x00;
enum BlinkingLEDStates{Start, S1, Wait2, S2, Wait3} BlinkingLEDStates;
void BlinkingLEDSM(){
        switch(BlinkingLEDStates){
                case S1:
                        BlinkingLEDStates = S2;
                        break;
                case S2:
                        BlinkingLEDStates = S1;
                        break;
                default:
                        BlinkingLEDStates = S1;
                        break;
        }
        switch(BlinkingLEDStates){
                case S1:
                        BlinkingLEDStates = 0x08;
                        break;
                case S2:
                        BlinkingLEDStates = 0x00;
                        break;
                default:
                        break;
        }
}

enum COMS{Input, Output} CombineStates;
void CombineLEDsSM(){
        switch(CombineStates){
                case Output:
                        PORTB = threeLEDs | blinkingLED;
                        CombineStates = Output;
                default:
                        CombineStates = Output;
                        break;
        }
}

unsigned char SOUNDER = 0x00;
enum SOUNDS {Wait, Press1, Press2} SOUND;
void Tick(){
        unsigned char temp = ~PINA & 0x04;
        switch(SOUND){
                case Wait:
                        if(temp == 0x04){
                                SOUND = Press1;
                        } else {
                                SOUND = Wait;
                        }
                        break;
                case Press1:
                        if(temp == 0x04){
                                SOUND = Press2;
                        } else {
                                SOUND = Wait;
                        }
                        break;
                case Press2:
                        if(temp == 0x04){
                                SOUND = Press1;
                        } else {
                                SOUND = Wait;
                        }
                        break;
                default:
                        SOUND = Wait;
                        break;
        }
        switch(SOUND){
                case Wait:
                        SOUNDER = 0x00;
                        break;
                case Press1:
                        SOUNDER = 0x10;
                        break;
                case Press2:
                        SOUNDER = 0x00;
                        break;
                default:
                        break;
        }
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
        unsigned long SM1 = 300; //statemachines 1-4
        unsigned long SM2 = 1000;
        unsigned long SM3 = 2;
        unsigned long SM4 = 2;
        const unsigned long timerPeriod = 2;
        TimerSet(timerPeriod);
        TimerOn();
        ThreeStates = ST1;
        BlinkingLEDStates = S1;
        CombineStates = Output;
        SOUND = Wait;
    while (1) {
        if(SM1 >= 300){
                ThreeLEDsSM();
                SM1 = 0;
        }
        if(SM2 >= 1000){
                BlinkingLEDSM();
                SM2 = 0;
        }
        if(SM3 >= 2){
                CombineLEDsSM();
                SM3 = 0;
        }
        if(SM4 >= 2){
                Tick();
                SM4 = 0;
        }
        while(!TimerFlag);
        TimerFlag = 0;
        SM1 += timerPeriod;
        SM2 += timerPeriod;
        SM3 += timerPeriod;
        SM4 += timerPeriod;
    }
    return 1;
}