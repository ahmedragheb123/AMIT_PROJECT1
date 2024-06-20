/*
 * LCD.c
 *
 * Created: 1/31/2023 7:41:08 PM
 *  Author: DELL
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000ul
#include <util/delay.h>
#include "IncFile1.h"
void command(char x){
	PORTD=(PORTD&0X0f)|(x&0Xf0);
	clear(PORTC,2);
	clear(PORTC,3);
	set(PORTC,4);
	_delay_us(1);
	clear(PORTC,4);
	_delay_us(200);
	PORTD=(PORTD&0X0f)|(x<<4);
	set(PORTC,4);
	_delay_us(1);
	clear(PORTC,4);
	_delay_ms(3);
}
void send(char y){
	PORTD=(PORTD&0x0f)|(y&0xf0);
	clear(PORTC,3);
	set(PORTC,2);
	set(PORTC,4);
	_delay_us(1);
	clear(PORTC,4);
	_delay_us(200);
	PORTD=(PORTD&0x0f)|(y<<4);
	set(PORTC,4);
	_delay_us(1);
	clear(PORTC,4);
	_delay_ms(3);
}