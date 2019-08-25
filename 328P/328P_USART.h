/*
 * 328_USART.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for Atmega328 visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#ifndef _328P_USART_H_
#define _328P_USART_H_


#include "stdutils.h"

#ifndef F_CPU
	#define F_CPU	16000000
#endif

#define USART0_REG		0xC0
#define USART_TIMEOUT	100	/* loop */

typedef struct _usart
{
	uint8 ucsr_a;		/* USART Control & Status Register A */
	uint8 ucsr_b;		/* USART Control & Status Register B */
	uint8 ucsr_c;		/* USART Control & Status Register C */
	uint8 reserved;
	uint8 ubrr_l;		/* USART Baud Rate Register Low */
	uint8 ubrr_h;		/* USART Baud Rate Register High */
	uint8 udr;		/* USART Data Register */
} usart;

void USART_Init(void);
bool USART_RxByte(uint8 *data);
void USART_TxByte(uint8 data);
void USART_RxByte_IT();
bool USART_RxBuffer(uint8 *buffer, uint16 len);
void USART_TxBuffer(uint8 *buffer, uint16 len);
bool USART_Available(void);

volatile usart * const usart0 = (void*)USART0_REG;
volatile uint8 usart0_rx_flag = 0;

/********************************************************************/
/********************************************************************/
ISR(USART_RX_vect)
{
	/* Disable Rx Interrupt */
	usart0->ucsr_b &= ~_BV(RXCIE0);
	usart0_rx_flag = 1;
}

void USART_Init()
{
	/* Enable U2X0 */
	usart0->ucsr_a |= _BV(U2X0);
	
	/* 115200 Baud rate, UBRR = (fosc / (8 * BAUD)) - 1 */
	/* 16MHz / (8 * 9600) - 1 = 16 */
	uint16 baudrate = F_CPU / 8 / 9600 - 1;
	usart0->ubrr_h = (baudrate >> 8) & 0xFF;
	usart0->ubrr_l = baudrate & 0xFF;
	
	/* 8-N-1 */
	usart0->ucsr_c |= _BV(UCSZ01) | _BV(UCSZ00);
	
	/* Enable Transmit + Receive */
	usart0->ucsr_b |= _BV(RXEN0) | _BV(TXEN0);
}

bool USART_RxByte(uint8 *data)
{
	uint16 loop = USART_TIMEOUT;
	do
	{
		if(usart0->ucsr_a & _BV(RXC0))
		{
			*data = usart0->udr;
			return true;
		}
		_delay_ms(1);
	} while (--loop);
	
	return false;
}

void USART_RxByte_IT()
{
	/* Enable Rx Interrupt */
	usart0->ucsr_b |= _BV(RXCIE0);
}

void USART_TxByte(uint8 data)
{
	while((usart0->ucsr_a & _BV(UDRE0)) == 0);
	usart0->udr = data;
}

bool USART_RxBuffer(uint8 *buffer, uint16 len)
{
	for(int i=0; i < len; i++)
	{
		if(!USART_RxByte(&buffer[i]))
		{
			return false;
		}
		
		/* DFPlayer dedicated code */
		if(i == 2 && buffer[2] != 0x06) return false;
	}
	
	return true;
}

void USART_TxBuffer(uint8 *buffer, uint16 len)
{
	for(int i=0; i < len; i++)
	{
		USART_TxByte(buffer[i]);
	}
}

bool USART_Available()
{
	if(usart0->ucsr_a & _BV(RXC0))
	{
		return true;
	}
	
	return false;
}
/********************************************************************/

#endif /* _328P_USART_H_ */
