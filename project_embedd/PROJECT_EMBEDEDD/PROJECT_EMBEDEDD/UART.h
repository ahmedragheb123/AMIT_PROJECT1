/*
 * UART.h
 *
 * Created: 1/31/2023 7:55:51 PM
 *  Author: DELL
 */ 

void uart_transmit(unsigned char k);
unsigned char uart_recieve(void);
void uart_send_string(char *arr2);
void USART_Init( unsigned int baud );