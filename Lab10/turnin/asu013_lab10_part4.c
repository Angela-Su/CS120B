/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise 4
 *	Exercise Description: Extend the previous exercise to allow a user to adjust the sound 
 *	frequency up or down using buttons connected to PA0 (up) and PA1 (down). Using our 1 ms 
 *	timer abstraction, the fastest you'll be able to pulse is 1 ms on and 1 ms off, meaning 
 *	500 Hz. 
 *
 * 
 * Demo Link: https://drive.google.com/file/d/1bWpphRTAQ4UAOGABwH7dKFZJATkMAO15/view?usp=sharing
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

int soundFreq = 0;
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

unsigned char SOUNDESet = 0x00;
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
                        PORTB = threeLEDs | blinkingLED | SOUNDESet;
                        CombineStates = Output;
                default:
                        CombineStates = Output;
                        break;
        }
}

int center = 0;
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
                        if(temp == 0x04 && center < soundFreq){
                                center++;
                                SOUND = Press2;
                        } else if(temp == 0x04 && soundFreq == 0){
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
                        center = 0;
                        SOUNDESet = 0x00;
                        break;
                case Press1:
                        SOUNDESet = 0x10;
                        break;
                case Press2:
                        SOUNDESet = 0x00;
                        break;
                default:
                        break;
        }
}

enum SOUND2S{SoundState, IncPress, DecPress} SOUND2;
void Sounds_Func(){
        unsigned temp2 = ~PINA & 0x03;
        switch(SOUND2){
                case SoundState:
                        if((temp2 == 0x02) && (soundFreq < 7)){
                                soundFreq++;
                                SOUND2 = IncPress;
                        } else if((temp2 == 0x01) && (soundFreq > 0)){
                                soundFreq--;
                                SOUND2 = DecPress;
                        } else {
                                SOUND2 = SoundState;
                        }
                        break;
                case IncPress:
                        if(temp2 == 0x02){
                                SOUND2 = IncPress;
                        } else {
                                SOUND2 = SoundState;
                        }
                        break;
                case DecPress:
                        if(temp2 == 0x01){
                                SOUND2 = DecPress;
                        } else {
                                SOUND2 = SoundState;
                        }
                        break;
                default:
                        break;
        }
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0xFF; PORTA = 0x00;
        DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
        unsigned long StateM1 = 300; //statemachines 1-4
        unsigned long StateM2 = 1000;
        unsigned long StateM3 = 1;
        unsigned long StateM4 = 1;
        unsigned long StateM5 = 1;
        const unsigned long timerPeriod = 1;
        TimerSet(timerPeriod);
        TimerOn();
        ThreeStates = State1;
        BlinkingLEDStates = S1;
        CombineStates = Output;
        SOUND = Wait;
        SOUND2 = SoundState;
    while (1) {
        if(StateM1 >= 300){
                ThreeLEDsSM();
                StateM1 = 0;
        }
        if(StateM2 >= 1000){
                BlinkingLEDSM();
                StateM2 = 0;
        }
        if(StateM3 >= 1){
                CombineLEDsSM();
                StateM3 = 0;
        }
        if(StateM4 >= 1){
                Tick();
                StateM4 = 0;
        }
        if(StateM5 >= 1){
                Sounds_Func();
                StateM5 = 0;
        }
        while(!TimerFlag);
        TimerFlag = 0;
        StateM1 += timerPeriod;
        StateM2 += timerPeriod;
        StateM3 += timerPeriod;
        StateM4 += timerPeriod;
        StateM5 += timerPeriod;
    }
    return 1;
}
