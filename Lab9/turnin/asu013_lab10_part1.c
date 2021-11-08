/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: Connect LEDs to PB0, PB1, PB2, and PB3. 
 *	In one state machine (ThreeLEDsSM), output to a shared variable (threeLEDs) the following 
 *	behavior: set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second 
 *	each. 
 *	In a second state machine (BlinkingLEDSM), output to a shared variable (blinkingLED) the 
 *	following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second. 
 *	In a third state machine (CombineLEDsSM), combine both shared variables and output to the 
 *	PORTB.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */

    /* Insert your solution below */
    while (1) {

    }
    return 1;
}
