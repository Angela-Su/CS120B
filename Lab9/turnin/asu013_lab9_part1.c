/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: Using the ATmega1284’s PWM functionality, design a system that uses 
 *	three buttons to select one of three tones to be generated on the speaker. When a button 
 *	is pressed, the tone mapped to it is generated on the speaker. Criteria:
 *	Use the tones C4, D4, and E4 from the table in the introduction section.
 *	When a button is pressed and held, the tone mapped to it is generated on the speaker.
 *	When more than one button is pressed simultaneously, the speaker remains silent. 
 *	When no buttons are pressed, the speaker remains silent.
 *	Demo Link: https://drive.google.com/file/d/1MtCrSqNFY_G3za_B3dwItSZsFix7d6jZ/view?usp=sharing
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
        static double current_frequency;
        if(frequency != current_frequency){
                if(!frequency){ 
			TCCR3B &= 0x08; 
		}
                else {
			TCCR3B |= 0x03; 
		}
                if(frequency < 0.954){
	 		OCR3A = 0xFFFF; 
		}
                else if(frequency > 31250){ 
			OCR3A = 0x0000; 
		}
                else {
			OCR3A = (short)(8000000 / (128*frequency)) - 1; 
		}
                TCNT3 = 0;
                current_frequency = frequency;
        }
}

void PWM_on(){
        TCCR3A = (1 << COM3A0);
        TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
        set_PWM(0);
}

void PWM_off(){
        TCCR3A = 0x00;
        TCCR3B = 0x00;
}

enum State{Init, Start, Press1, Press2, Press3} State;
void Tick(){
        unsigned char temp = ~PINA & 0x07;
        switch(State){
                case Init:
                        State = Start;
                        break;
                case Start:
                        if(temp == 0x01){
                                State = Press1;
                        } 
			else if(temp == 0x02){
                                State = Press2;
                        } 
			else if(temp == 0x04){
                                State = Press3;
                        } 
			else {
                                State = Start;
                        }
                        break;
                case Press1:
                        if(temp == 0x01){
                                State = Press1;
                        } 
			else {
                                State = Start;
                        }
                        break;
                case Press2:
                        if(temp == 0x02){
                                State = Press2;
                        } 
			else {
                                State = Start;
                        }
                        break;
                case Press3:
                        if(temp == 0x04){
                                State = Press3;
                        } 
			else {
                                State = Start;
                        }
                        break;
                default:
                        State = Start;
                        break;
        }
        switch(State){
                case Init:
                        break;
                case Start:
                        set_PWM(0);
                        break;
                case Press1:
                        set_PWM(261.63);
                        break;
                case Press2:
                        set_PWM(293.66);
                        break;
                case Press3:
                        set_PWM(329.63);
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
        PWM_on();
        State = Init;
    while (1) {
        Tick();
    }
    return 1;
}
