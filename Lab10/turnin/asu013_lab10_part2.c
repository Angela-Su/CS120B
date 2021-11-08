/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise 2
 *	Exercise Description: Modify the above example so the threeLEDs light for 300 ms, while 
 *	blinkingLED’s LED still blinks 1 second on and 1 second off.
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


int main(void) {
    /* Insert DDR and PORT initializations */
        DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
        unsigned long StateM1 = 300; //statemachines 1-3
        unsigned long StateM2 = 1000;
        unsigned long StateM3 = 100;
        const unsigned long timerPeriod = 100;
        TimerSet(timerPeriod);
        TimerOn();
        ThreeStates = ST1;
        BlinkingLEDStates = S1;
        CombineStates = Output;
    while (1) {
        if(StateM1 >= 300){
                ThreeLEDsSM();
                StateM1 = 0;
        }
        if(StateM2 >= 1000){
                BlinkingLEDSM();
                StateM2 = 0;
        }
        if(StateM3 >= 100){
                CombineLEDsSM();
                StateM3 = 0;
        }
        while(!TimerFlag);
        TimerFlag = 0;
        StateM1 += timerPeriod;
        StateM2 += timerPeriod;
        StateM3 += timerPeriod;
    }
    return 1;
}