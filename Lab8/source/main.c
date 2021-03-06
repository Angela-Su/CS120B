/*	Author: Angela
 *  Partner(s) Name: 
 *	Lab Section:022
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: Design a system where an LED is illuminated only if enough light is 
 *	detected from the photo resistor. Criteria:
 *	If the result of the ADC is >= MAX/2, the LED is illuminated.
 *	If the result of the ADC is < MAX/2, the LED is turned off.

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	ADC_init();
	unsigned short max = 0x0133;
        unsigned short unit = max/8;

    /* Insert your solution below */
    while (1) {
	unsigned short temp = ADC;
	if(temp <= unit){
                PORTB = 0x01;
        } else if(temp <= unit*2){
                PORTB = 0x03;
        } else if(temp <= unit*3){
                PORTB = 0x07;
        } else if(temp <= unit*4){
                PORTB = 0x0F;
        } else if(temp <= unit*5){
                PORTB = 0x1F;
        } else if(temp <= unit*6){
                PORTB = 0x3F;
        } else if(temp <= unit*7){
                PORTB = 0x7F;
        } else if(temp <= unit*8){
                PORTB = 0xFF;        
	} else {
                PORTB = 0x00;
        }
    }
    return 1;
}
