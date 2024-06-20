/*
 * ADC.c
 *
 * Created: 1/31/2023 7:42:33 PM
 *  Author: DELL
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000ul
#include <util/delay.h>
#include "IncFile1.h"
void ADC_intilization(int adc_number){
	ADMUX|=adc_number;
	set(ADMUX,REFS0);
	set(ADCSRA,ADEN);
	set(ADCSRA,ADPS2);
}
int ADC_READ(){
	int x=0;
	set(ADCSRA,ADSC);
	while(read(ADCSRA,ADIF)==0){   //UNTIL THE FLAGE IS ON TO LET TIME TO ADC
	}
	_delay_us(10);
	x=(int)ADCL+(int)ADCH*256;
	return (x);
}