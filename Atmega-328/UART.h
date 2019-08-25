/*
 * UART.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for AVR Microcontrollers visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#ifndef UART_H
#define UART_H

void UART_Init(void);								//Initialize UART Registers
void UART_SendByte(const unsigned char data);		//send byte of data 
uint16 UART_RecieveByte(void);				//Receive byte of data
void UART_SendString(const unsigned char *Str);		//Send full String
void UART_ReceiveString(unsigned char *Str);		// Receive string until #

void UART_Init(void)
{	
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) ; //enable UART as transmitter and receiver.
	 
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); //8-bit data, NO parity, one stop bit and asynch
	
	/* baud rate=9600 & Fosc=1MHz -->  UBBR=( Fosc / (16 * baud rate) ) - 1 = 5 */  /* 103 for 16MHz */ /* 51 for 8MHz*/
	UBRR0H = 0;
	UBRR0L = 103;
}

void UART_SendByte(const unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))){} //UDRE flag is set when the buffer is empty and ready for transmitting a new byte so wait until this flag is set to one.
	UDR0 = data;
}

uint16 UART_RecieveByte(void)
{
	while(!(UCSR0A & (1<<RXC0))){} //RXC flag is set when the UART receive data so until this flag is set to one
	return UDR0;
}

void UART_SendString(const unsigned char *Str)
{
	unsigned char i = 0;
	while(Str[i] != '\0')
	{
		UART_SendByte(Str[i]);
		i++;
	}
}

void UART_ReceiveString(unsigned char *Str)
{
	unsigned char i = 0;
	Str[i] = UART_RecieveByte();
	while(Str[i] != '$')
	{
		i++;
		Str[i] = UART_RecieveByte();
	}
	Str[i] = '\0';
}

#endif
