/*	Author: Angela
 *  Partner(s) Name: 
 *	Lab Section:022
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: Design a system where an LED is illuminated only if enough light is 
 *	detected from the photo resistor. Criteria:
 *	If the result of the ADC is >= MAX/2, the LED is illuminated.
 *	If the result of the ADC is < MAX/2, the LED is turned off.
 *	Demo Link: https://drive.google.com/file/d/1MT9gj4NIzMARtNONRlLHAocIebLXXH1V/view?usp=sharing
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
	unsigned short min = 0x0013;
        unsigned short max = 0x0125;
        unsigned short compareVal = max/2;
    /* Insert your solution below */
    while (1) {
	unsigned short temp = ADC;
	if(temp >= compareVal){
		PORTB = 0x01;
	}
	else{
		PORTB = 0x00;
	}
    }
    return 1;
}
