/*
 * SPI.h
 * Atmega8
 * Se7S_92
 * Last modification 8-24-2019
 */ 

#include "stdutils.h"

#ifndef SPI_H
#define SPI_H

void SPI_InitMaster(void); 
void SPI_InitSlave(void);
void SPI_SendByte(unsigned char data); 
unsigned char SPI_RecieveByte(void);
void SPI_SendString(const unsigned char *Str);
void SPI_ReceiveString(char *Str);

void SPI_InitMaster(void) 
{
	/******** Configure SPI Master Pins *********
	 * SS(PB4)   --> Output
	 * MOSI(PB5) --> Output 
	 * MISO(PB6) --> Input
	 * SCK(PB7) --> Output
	 ********************************************/
	DDRB |= (1<<PB4);
	DDRB |= (1<<PB5);
	DDRB &= ~(1<<PB6);
	DDRB |= (1<<PB7);
	
	SPCR = (1<<SPE) | (1<<MSTR); // enable SPI + enable Master + choose SPI clock = Fosc/4
}

void SPI_InitSlave(void)
{ 
	/******** Configure SPI Slave Pins *********
	 * SS(PB4)   --> Input
	 * MOSI(PB5) --> Input
	 * MISO(PB6) --> Output
	 * SCK(PB7) --> Input
	 ********************************************/
	DDRB &= ~(1<<PB4);  
	DDRB &= ~(1<<PB5);
	DDRB |= (1<<PB6);
	DDRB &= ~(1<<PB7);
	SPCR = 1<<SPE ; // Enable SPI 
}

void SPI_SendByte(const unsigned char data) 
{
	SPDR = data; //send data by SPI
	while(!(SPSR & (1<<SPIF))){} //wait until SPI interrupt flag=1 (data is sent correctly)								
}

unsigned char SPI_RecieveByte(void)
{
   while(!(SPSR & (1<<SPIF))){} //wait until SPI interrupt flag=1(data is receive correctly)	
   return SPDR; //return the received byte from SPI data register
}
void SPI_SendString(const unsigned char *Str)
{
	unsigned char i = 0;
	while(Str[i] != '\0')
	{
		SPI_SendByte(Str[i]);
		i++;
	}
}

void SPI_ReceiveString(char *Str)
{
	unsigned char i = 0;
	Str[i] = SPI_RecieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = SPI_RecieveByte();
	}
	Str[i] = '\0';
}

#endif