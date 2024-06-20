/*
 * eeprom.c
 *
 * Created: 2/2/2023 9:24:01 PM
 *  Author: DELL
 */ 
#define F_CPU 1000000ul
#define SCL_CLK 10000
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "IncFile1.h"
void EEPROM_Write(char page_number,char address, char data)
{
	char page_add = (page_number<<1)+0xA0;
	I2C_Start(page_add);			/* Start I2C with device write address */
	I2C_Write(address);			/* Write start memory address for data write */
	I2C_Write(data);
	I2C_Stop();					/* Stop I2C */
	
}
void EEPROM_Read(char page_number,char address,char *data)
{
	char page_write_add = (page_number<<1)+0xA0;
	char page_read_add  = page_write_add + 1;
	_delay_ms(10);
	I2C_Start(page_write_add);							/* Start I2C with device write address */
	I2C_Write(address);							/* Write start memory address */
	I2C_Repeated_Start(page_read_add);			/* Repeat start I2C SLA+R */
	*data = I2C_Read_Ack();
	I2C_Stop();									/* Stop I2C */
}