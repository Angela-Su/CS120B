/*	Author: Angela Su
 *  Partner(s) Name: 
 *	Lab Section: 022
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: Use the LCD* code, along with a button and/or time delay to 
 *  display the message "CS120B is Legend... wait for it DARY!" The string will not fit 
 *  on the display all at once, so you will need to come up with some way to paginate 
 *  or scroll the text.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "header/bit.h"
#include "header/io.h"
#include "io.h"
#include "header/keypad.h"
#include "header/lcd_8bit_task.h"
#include "header/queue.h"
#include "header/scheduler.h"
#include "header/seven_seg.h"
#include "header/stack.h"
#include "header/timer.h"
#include "header/usart.h"
#endif

/*enum display_States{display_display};
int displaySMTick(int state){
    unsigned char output;
    switch(state){
        case display_display :
            state = display_display;
            break;
        default:
            break;
    }
    switch(state){
        case display_display:
            output = led0_output |led1_output << 1;
            break;
        default:
            break;
    }
    PORTB = output;
    return state;
}*/
enum displayTick{start, display}states;
unsigned char MSGarr[16];
char* msg = "CS120B is Legend... wait for it DARY!";
void displayMSGTick(){
    unsigned char counter = 0;
    switch(states){
        case start:
        case display:
        default:
            states = display;
            break;
    }
    switch(states){
        case start:
            break;
        case display:
            for (int i = 0; i < 16; i++) {
		        MSGarr[i] = msg[(counter+i) % 38];
	        }
	        counter = (counter+1) % 38;
	        LCD_DisplayString(1, MSGarr);
            break;
    }
	return states;
}

/*enum GetKey{key};
int GetKeyTick(int state){
    unsigned char x;
    x = GetKeypadKey();
    switch(state){
        case key : 
            switch(x){
                case '\0' : PORTB = 0x1F; break;
                case '1' : PORTB = 0x01; break;
                case '2' : PORTB = 0x02; break;
                case '3' : PORTB = 0x03; break;
                case '4' : PORTB = 0x04; break;
                case '5' : PORTB = 0x05; break;
                case '6' : PORTB = 0x06; break;
                case '7' : PORTB = 0x07; break;
                case '8' : PORTB = 0x08; break;
                case '9' : PORTB = 0x09; break;
                case 'A' : PORTB = 0x0A; break;
                case 'B' : PORTB = 0x0B; break;
                case 'C' : PORTB = 0x0C; break;
                case 'D' : PORTB = 0x0D; break;
                case '*' : PORTB = 0x0E; break;
                case '0' : PORTB = 0x00; break;
                case '#' : PORTB = 0x0F; break;
                default : PORTB = 0x1B; break;
            }
            state = key;
        default : break;
    }
    return state;
}
*/

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    /*static task task1, task2, task3, task4, task5;
    task *task[] = {&task1, &task2, &task3, &task4, &task5};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &pauseButtonSMTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &toggleLED0SMTick;

    task3.state = start;
    task3.period = 1000;
    task3.elapsedTime = task3.period;
    task3.TickFct = &toggleLED1SMTick;

    task4.state = start;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &displaySMTick;

    task5.state = start;
    task5.period = 100;
    task5.elapsedTime = task5.period;
    task5.TickFct = &GetKeyTick;*/

    static task task1;
    task *task[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &displayMSGTick;

    TimerSet(1);
    TimerOn();
    LCD_init();

    unsigned short i;
    while (1) {
        for(i = 0; i < numTasks; i++){
            if(tasks[i]->elapsedTime == tasks[i]->period){
                tasks[i]->state = tasks[i]->TickFct{tasks[i]->state};
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elasedTime += 1;
        }
        while (!TimerFlag){
            TimerFlag = 0;
        }
    }
    return 0;
}
