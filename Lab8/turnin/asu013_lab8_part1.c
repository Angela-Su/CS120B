/*	Author: Angela
 *  Partner(s) Name: 
 *	Lab Section:022
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: Make sure your breadboard is wired according to the prelab. The potentiometer is used 
 *	to adjust the voltage supplied to the microcontroller for ADC . Design a system that reads the 10-bit ADC 
 *	result from the ADC register, and displays the result on a bank of 10 LEDs.
 *	Demo Link: https://drive.google.com/file/d/1M-7h61in1yWSwgUC2TwmDmMPuwNBEyc3/view?usp=sharing
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
    /* Insert your solution below */
    while (1) {
	unsigned short temp = ADC;
	unsigned char temp2 = (char)temp;
	unsigned char temp3 = (char)(temp >> 8);
	unsigned char temp4 = temp3 & 0x03;
	PORTB = temp2;
	PORTD = temp4;
    }
    return 1;
}
