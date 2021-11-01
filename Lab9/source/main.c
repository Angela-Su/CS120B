/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: Using the ATmega1284’s PWM functionality, design a system where the 
 *	notes: C4, D, E, F, G, A, B, and C5,  from the table at the top of the lab, can be 
 *	generated on the speaker by scaling up or down the eight note scale. Three buttons are   
 *	used to control the system. One button toggles sound on/off. The other two buttons scale 
 *	up, or down, the eight note scale. Criteria:
 *	The system should scale up/down one note per button press.
 *	When scaling down, the system should not scale below a C.
 *	When scaling up, the system should not scale above a C.

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
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

unsigned char isOn = 0x00;
enum State{Init, Off, N1, N2, N3, N4, N5, N6, N7, N8, Wait0, Wait1, Wait2, Wait3, Wait4, Wait5, Wait6, Wait7, Wait8} State;
void Tick_Func(){
        unsigned char temp = ~PINA & 0x07;
        if(temp == 0x01){
                if(isOn == 0x00){
                        isOn = 0x01;
                } else {
                        isOn = 0x00;
                }
        }
        if(isOn == 0x01){
        switch(SM_STATE){
                case SM_Init:
                        SM_STATE = SM_N1;
                        break;
                case SM_Wait0:
                        if(temp == 0x04){
                                SM_STATE = SM_Wait0;
                        } else {
                                SM_STATE = SM_N1;
                        }
                        break;
                case SM_N1:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait1;
                        } else {
                                SM_STATE = SM_N1;
                        }
                        break;
                case SM_Wait1:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait1;
                        } else {
                                SM_STATE = SM_N2;
                        }
                        break;
                case SM_N2:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait2;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait0;
                        } else {
                                SM_STATE = SM_N2;
                        }
                        break;
                case SM_Wait2:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait2;
                        } else {
                                SM_STATE = SM_N3;
                        }
                        break;
                case SM_N3:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait3;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait1;
                        } else {
                                SM_STATE = SM_N3;
                        }
                        break;
                case SM_Wait3:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait3;
                        } else {
                                SM_STATE = SM_N4;
                        }
                        break;
                case SM_N4:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait4;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait2;
                        } else {
                                SM_STATE = SM_N4;
                        }
                        break;
                case SM_Wait4:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait4;
                        } else {
                                SM_STATE = SM_N5;
                        }
                        break;
                case SM_N5:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait5;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait3;
                        } else {
                                SM_STATE = SM_N5;
                        }
                        break;
                case SM_Wait5:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait5;
                        } else {
                                SM_STATE = SM_N6;
                        }
                        break;
                case SM_N6:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait6;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait4;
                        } else {
                                SM_STATE = SM_N6;
                        }
                        break;
                case SM_Wait6:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait6;
                        } else {
                                SM_STATE = SM_N7;
                        }
                        break;
                case SM_N7:
                        if(temp == 0x02){
                                SM_STATE = SM_Wait7;
                        } else if(temp == 0x04){
                                SM_STATE = SM_Wait5;
                        } else {
                                SM_STATE = SM_N7;
                        }
                        break;
                case SM_Wait7:
                        if(temp == 0x02 || temp == 0x04){
                                SM_STATE = SM_Wait7;
                        } else {
                                SM_STATE = SM_N8;
                        }
                        break;
                case SM_N8:
                        if(temp == 0x04){
                                SM_STATE = SM_Wait6;
                        } else {
                                SM_STATE = SM_N8;
                        }
                        break;
                default:
                        SM_STATE = SM_Init;
                        break;
        }
        switch(SM_STATE){
                case SM_Init:
                        break;
                case SM_N1:
                        set_PWM(261.63);
                        break;
                case SM_N2:
                        set_PWM(293.66);
                        break;
                case SM_N3:
                        set_PWM(329.63);
                        break;
                case SM_N4:
                        set_PWM(349.23);
                        break;
                case SM_N5:
                        set_PWM(392);
                        break;
                case SM_N6:
                        set_PWM(440);
                        break;
                case SM_N7:
                        set_PWM(493.88);
                        break;
                case SM_N8:
                        set_PWM(523.25);
                        break;
                default:
                        break;
        }
        }
        if(isOn == 0x00){
                set_PWM(0);
        }
}



int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF;
        DDRB = 0x40; PORTB = 0x00;
    /* Insert your solution below */
        PWM_on();
        SM_STATE = SM_Init;
    while (1) {
        Tick_Func();
    }
    return 1;
}
