/*	Author: Angela Su
 *  Partner(s) Name: N/A
 *	Lab Section: 022
 *	Assignment: Lab #2  Exercise #2
 *	Exercise Description:Port A's pins 3 to 0, each connect to a parking space sensor, 1
 *	meaning a car is parked in the space, of a four-space parking lot. Write a program that 
 *	outputs in binary on port C the number of available spaces (Hint: declare a variable 
 *	"unsigned char cntavail;" you can assign a number to a port as follows: 
 *	PORTC = cntavail;).
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tempD = 0x00; // Temporary variable to hold the value of D
	unsigned char tempC = 0x00; // Temporary variable to hold the value of C
	unsigned char tempB = 0x00; // Temporary variable to hold the value of B
	unsigned char tempA = 0x00; // Temporary variable to hold the value of A
	unsigned char cntavail = 0x00;	
	PORTC = cntavail;

while(1) {
	tempA = PINA & 0x01;
	tempB = PINA & 0x02;	
	tempC = PINA & 0x04;
	tempD = PINA & 0x08;

	if(tempA != 0x01){
		cntavail++;
	}
	if(tempB != 0x02){
		cntavail++;
	}
	if(tempC != 0x04){
		cntavail++;
	}
	if(tempD != 0x08){
		cntavail++;
	}
	PORTC = cntavail;
	cntavail = 0;
}
	return 0;
}


