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
enum State{Init, Off, C4, D4, E4, F4, G4, A4, B4, C5, Wait0, Wait1, Wait2, Wait3, Wait4, Wait5, Wait6, Wait7, Wait8} State;
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
        switch(State){
                case Init:
                        State = C4;
                        break;
                case Wait0:
                        if(temp == 0x04){
                                State = Wait0;
                        } else {
                                State = C4;
                        }
                        break;
                case C4:
                        if(temp == 0x02){
                                State = Wait1;
                        } else {
                                State = C4;
                        }
                        break;
                case Wait1:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait1;
                        } else {
                                State = D4;
                        }
                        break;
                case D4:
                        if(temp == 0x02){
                                State = Wait2;
                        } else if(temp == 0x04){
                                State = Wait0;
                        } else {
                                State = D4;
                        }
                        break;
                case Wait2:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait2;
                        } else {
                                State = E4;
                        }
                        break;
                case E4:
                        if(temp == 0x02){
                                State = Wait3;
                        } else if(temp == 0x04){
                                State = Wait1;
                        } else {
                                State = E4;
                        }
                        break;
                case Wait3:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait3;
                        } else {
                                State = F4;
                        }
                        break;
                case F4:
                        if(temp == 0x02){
                                State = Wait4;
                        } else if(temp == 0x04){
                                State = Wait2;
                        } else {
                                State = F4;
                        }
                        break;
                case Wait4:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait4;
                        } else {
                                State = G4;
                        }
                        break;
                case G4:
                        if(temp == 0x02){
                                State = Wait5;
                        } else if(temp == 0x04){
                                State = Wait3;
                        } else {
                                State = G4;
                        }
                        break;
                case Wait5:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait5;
                        } else {
                                State = A4;
                        }
                        break;
                case A4:
                        if(temp == 0x02){
                                State = Wait6;
                        } else if(temp == 0x04){
                                State = Wait4;
                        } else {
                                State = A4;
                        }
                        break;
                case Wait6:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait6;
                        } else {
                                State = B4;
                        }
                        break;
                case B4:
                        if(temp == 0x02){
                                State = Wait7;
                        } else if(temp == 0x04){
                                State = Wait5;
                        } else {
                                State = B4;
                        }
                        break;
                case Wait7:
                        if(temp == 0x02 || temp == 0x04){
                                State = Wait7;
                        } else {
                                State = C5;
                        }
                        break;
                case C5:
                        if(temp == 0x04){
                                State = Wait6;
                        } else {
                                State = C5;
                        }
                        break;
                default:
                        State = Init;
                        break;
        }
        switch(State){
                case Init:
                        break;
                case C4:
                        set_PWM(261.63);
                        break;
                case D4:
                        set_PWM(293.66);
                        break;
                case E4:
                        set_PWM(329.63);
                        break;
                case F4:
                        set_PWM(349.23);
                        break;
                case G4:
                        set_PWM(392);
                        break;
                case A4:
                        set_PWM(440);
                        break;
                case B4:
                        set_PWM(493.88);
                        break;
                case C5:
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
        State = Init;
    while (1) {
        Tick_Func();
    }
    return 1;
}
