/*
 * PROJECT_EMBEDEDD.c
 *
 * Created: 1/26/2023 1:59:17 PM
 * Author : DELL
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000ul
#include <util/delay.h>
#include "IncFile1.h"
#include "LCD.h"
#include "timer.h"
#include "UART.h"
#include "ADC.h"
volatile char x=60;   // initial set temperature
volatile char k=0;   // password variable
ISR(USART_RXC_vect){  //u_art interrupt 
	k = UDR ;
}
int main(void)
{
	char w=0;
	I2C_Init();			/* Initialize I2C */
	EEPROM_Read(1,0x12,&w);
	if(w<35||w>75){}
	else{
		x=w;
	}
	Timer_int(); // Timer
	int temp=0;  // used for toggle 
	int j=0;     // used for password
	sei();       // global interrupt 
	ADC_intilization(0); //ADC INTILIZATION
	char arr[12]={"set temp is"}; // ARRAY used for write on lcd
	char arr1[5];                 // used for variation of set temperature
	int y=0;                      // resolution of ADC(0-1023)
	int z;                        // Current temperature
	char c=0;                     // counter for timer
	char a=0;                     // first number of the password
	char b=0;                     // second number of the password
	char counter=0;
	USART_Init(12);               
	uart_send_string("enter your password    "); 
	// declaration of inputs and outputs 
	DDRC=0xff;                   
	DDRD=0xff;             
	clear(DDRB,0); 
	clear(DDRB,7);
	clear(DDRA,0);
	set(DDRA,6);
	set(DDRA,7);
	set(DDRA,5);
	 // commands for LCD
	command(0x02);                 
	command(0x28);
	command(0x0C);
	command(0x06);
	command(0x01);
	command(0x80);
	while(1){
		// conditions of the password
		if(k=='1'){
			a=k;
		}
		if(k=='2'){
			b=k;                        
		}
		if(a=='1'&& b=='2'){
			if(j==0){
				uart_send_string("  Right password,please go to ON/OFF button");
			}
			j++;
		if(read(PINB,7)==1){                //condition of the ON/OFF button
			
			y=ADC_READ();     
			if (read(PINB,0)==1)            //Up button
			{
				while(read(PINB,0)==1){};
					if(counter==0){                     //first press enter setting mode
						for(int i=0; i<12 ; i++){
							send(arr[i]);            // write on lcd
						}
						sprintf(arr1,"%d",x);
						for(int i=0; i<5; i++){
							send(arr1[i]);           // write on lcd
						}
						command(0x02);
						counter++;
					}
				else{	                                     //if second or third .....etc press
				x=x+5;
				EEPROM_Write(1,0x12,x);
				if(x>75){
					x=75;                                  //maximum allowable set temperature
					EEPROM_Write(1,0x12,x);                  
				}
				for(int i=0; i<12 ; i++){
					send(arr[i]);            // write on lcd
				}
				sprintf(arr1,"%d",x);
				for(int i=0; i<5; i++){
					send(arr1[i]);           // write on lcd
				}
				command(0x02);
			}
				}
			 else if(read(PINB,1)==1){         //Down button
				while(read(PINB,1)==1){};
					if(counter==0){                                //first press enter setting mode
						for(int i=0; i<12 ; i++){
							send(arr[i]);            // write on lcd
						}
						sprintf(arr1,"%d",x);
						for(int i=0; i<5; i++){
							send(arr1[i]);           // write on lcd
						}
						command(0x02);
						counter++;
					}
				else{                                        //if second or third .....etc press
				x=x-5;
				EEPROM_Write(1,0x12,x);
				if(x<35){
					x=35;                                       //minimum allowable set temperature
					EEPROM_Write(1,0x12,x);                    
				}
				for(int i=0; i<12 ; i++){
					send(arr[i]);
				}
				sprintf(arr1,"%d",x);
				for(int i=0; i<5; i++){
					send(arr1[i]);
				}
				command(0x02);
			}
				}
			z=(0.4885993485*y);                 // linear equation between y(0-307)resolution  and z(0-150) Celsius
				if(z==x-5){
				set(PORTA,7);                       //Turn on heater
				clear(PORTA,6);                     //turn off cooler
				temp=1;                             // used for toggle the led
			}
			 if(z==x+5){
				set(PORTA,6);                    //Turn on cooler
				clear(PORTA,7);                  //turn off heater
				set(PORTA,5);                    // turn on led
				temp=0;                          // turn off toggle
			}
			while((read(TIFR,TOV0))==0){        //timer
				
			}
			c++;
			if(c==20){                         // timer for toggle led (every one second)
				if(z>=x-5&&z<x+5&&temp==1){
					toggle(PORTA,5);
					c=0;
				}
			}
			if(c==100){                        // timer to clear lcd (every 5 seconds if the up and down buttons are not pressed)
				if(((read(PINB,0))==0)&&((read(PINB,1))==0)){
					command(0x01);
					c=0;
				}
			}
			 set(TIFR,TOV0);                 
			}   
			else if(read(PINB,7)==0){    //turn off the system
				command(0x01);           // clear the lcd
			}
			}
	}
}