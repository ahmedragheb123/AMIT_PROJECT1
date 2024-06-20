/*
 * UART.c
 *
 * Created: 1/31/2023 7:45:08 PM
 *  Author: DELL
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000ul
#include <util/delay.h>
#include "IncFile1.h"

void uart_transmit(unsigned char k){
	while(read(UCSRA,UDRE)==0){
		;
	}
	UDR=k;
}
unsigned char uart_recieve(void){
	while(read(UCSRA,RXC)==0){
		;
	}
	return UDR;
}
void uart_send_string(char *arr2){
	unsigned char i=0;
	while(arr2[i]!=0){
		uart_transmit(arr2[i]);
		i++;
	}
}
void USART_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baud>>8);
	UBRRL = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	set(UCSRB,RXCIE);
	/* Set frame format: 8 bit data, 1stop bits */
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}
