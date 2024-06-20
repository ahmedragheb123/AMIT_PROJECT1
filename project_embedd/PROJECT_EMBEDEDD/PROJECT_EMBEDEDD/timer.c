/*
 * timer.c
 *
 * Created: 1/31/2023 7:39:49 PM
 *  Author: DELL
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000ul
#include <util/delay.h>
#include "IncFile1.h"
void Timer_int(void){
	TCNT0=0;                   //initial value of timer register
	set(TCCR0,CS02);           // for prescaler=256
}