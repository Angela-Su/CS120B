/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise 3
 *	Exercise Description: To the previous exercise's implementation, connect your speaker's 
 *	red wire to PB4 and black wire to ground. Add a third task that toggles PB4 on for 2 ms 
 *	and off for 2 ms as long as a switch on PA2 is in the on position. Don’t use the PWM for 
 *	this task.
 *
 * 
 * Demo Link: https://drive.google.com/file/d/1bnYKUOImaLjBSrXR3zdZIpJG3c4qLr-h/view?usp=sharing
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
enum ThreeStates{Init, State1, State2, State3, Restart} ThreeStates;
void ThreeLEDsSM(){
        switch(ThreeStates){
                case State1:
                        ThreeStates = State2;
                        break;
                case State2:
                        ThreeStates = State3;
                        break;
                case State3:
                        ThreeStates = State1;
                        break;
                default:
                        ThreeStates = State1;
                        break;
        }
        switch(ThreeStates){
                case State1:
                        threeLEDs = 0x01;
                        break;
                case State2:
                        threeLEDs = 0x02;
                        break;
                case State3:
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
                        blinkingLED = 0x08;
                        break;
                case S2:
                        blinkingLED = 0x00;
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

unsigned char SOUNDSet = 0x00;
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
                        SOUNDSet = 0x00;
                        break;
                case Press1:
                        SOUNDSet = 0x10;
                        break;
                case Press2:
                        SOUNDSet = 0x00;
                        break;
                default:
                        break;
        }
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
        unsigned long StateM1 = 300; //statemachines 1-4
        unsigned long StateM2 = 1000;
        unsigned long StateM3 = 2;
        unsigned long StateM4 = 2;
        const unsigned long timerPeriod = 2;
        TimerSet(timerPeriod);
        TimerOn();
        ThreeStates = State1;
        BlinkingLEDStates = S1;
        CombineStates = Output;
        SOUND = Wait;
    while (1) {
        if(StateM1 >= 300){
                ThreeLEDsSM();
                StateM1 = 0;
        }
        if(StateM2 >= 1000){
                BlinkingLEDSM();
                StateM2 = 0;
        }
        if(StateM3 >= 2){
                CombineLEDsSM();
                StateM3 = 0;
        }
        if(StateM4 >= 2){
                Tick();
                StateM4 = 0;
        }
        while(!TimerFlag);
        TimerFlag = 0;
        StateM1 += timerPeriod;
        StateM2 += timerPeriod;
        StateM3 += timerPeriod;
        StateM4 += timerPeriod;
    }
    return 1;
}