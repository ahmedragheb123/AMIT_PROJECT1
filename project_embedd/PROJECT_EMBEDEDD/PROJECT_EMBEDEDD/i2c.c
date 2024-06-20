/*
 * i2c.c
 *
 * Created: 2/2/2023 9:21:30 PM
 *  Author: DELL
 */ 
#define F_CPU 1000000ul
#define SCL_CLK 10000
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "IncFile1.h"
//SCL frequency =  (CPU CLK frequency)/(16+2(TWBR)*4^TWPS )
// f/sf =16+2(TWBR)*4^TWPS
//f/sf-16 =2(TWBR)*4^TWPS
//(f/sf-16)/4^TWPS*2 = TWBR
/* Define bit rate */
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))
void I2C_Init()
{
	TWBR = BITRATE(TWSR=0x00);	// Get bit rate register value by formula
}
char I2C_Start(char address)
{
	char status;
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);	/* Enable TWI, generate START					*/
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job		*/
	status=TWSR&0xF8;						/* Read TWI status register						*/
	if(status!=0x08)						/* Check weather START transmitted or not?		*/
	return 0;								/* Return 0 to indicate start condition fail	*/
	TWDR= address;							/* Write SLA+W in TWI data register				*/
	TWCR=(1<<TWEN)|(1<<TWINT);				/* Enable TWI & clear interrupt flag			*/
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job		*/
	status=TWSR&0xF8;						/* Read TWI status register */
	if(status==0x18)						/* Check for SLA+W transmitted &ack received	*/
	return 1;								/* Return 1 to indicate ack received			*/
	if(status==0x20)						/* Check for SLA+W transmitted &nack received	*/
	return 2;								/* Return 2 to indicate nack received			*/
	else
	return 3;								/* Else return 3 to indicate SLA+W failed		*/
}
char I2C_Repeated_Start(char read_address)
{
	char status;							/* Declare variable								*/
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);	/* Enable TWI, generate start					*/
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job		*/
	status=TWSR&0xF8;						/* Read TWI status register						*/
	if(status!=0x10)						/* Check for repeated start transmitted			*/
	return 0;								/* Return 0 for repeated start condition fail	*/
	TWDR=read_address;						/* Write SLA+R in TWI data register				*/
	TWCR=(1<<TWEN)|(1<<TWINT);				/* Enable TWI and clear interrupt flag			*/
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job		*/
	status=TWSR&0xF8;						/* Read TWI status register						*/
	if(status==0x40)						/* Check for SLA+R transmitted &ack received	*/
	return 1;								/* Return 1 to indicate ack received			*/
	if(status==0x48)						/* Check for SLA+R transmitted &nack received	*/
	return 2;								/* Return 2 to indicate nack received			*/
	else
	return 3;								/* Else return 3 to indicate SLA+W failed		*/
}
char I2C_Write(char data)	/* I2C write function */
{
	char status;				/* Declare variable */
	TWDR=data;					/* Copy data in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	status=TWSR&0xF8;			/* Read TWI status register */
	if(status==0x28)			/* Check for data transmitted &ack received */
	return 0;					/* Return 0 to indicate ack received */
	if(status==0x30)			/* Check for data transmitted &nack received */
	return 1;					/* Return 1 to indicate nack received */
	else
	return 2;					/* Else return 2 for data transmission failure */
}
char I2C_Read_Ack()
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);	/* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job */
	return TWDR;							/* Return received data */
}
char I2C_Read_Nack()
{
	TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;				/* Return received data */
}
void I2C_Stop()								/* I2C stop function */
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);	/* Enable TWI, generate stop */
	while(TWCR&(1<<TWSTO));					/* Wait until stop condition execution */
}