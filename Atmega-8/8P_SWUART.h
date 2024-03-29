﻿/*
 * software UART library for ATMEGA8
 * 8P_SWUART.h
 * Atmega8
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for AVR Microcontrollers visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#include "stdutils.h"

#ifndef __8P_SWUART_H_
#define __8P_SWUART_H_

#ifndef F_CPU
	#define F_CPU	16000000UL
#endif

#define SWU_DDR		DDRB
#define SWU_PORT	PORTB
#define SWU_PIN		PINB
#define SWU_RX		PB0
#define SWU_TX		PB1

#ifndef BAUDRATE
	#define BAUDRATE	9600
#endif

#define BIT_TIME	(uint16)(F_CPU / BAUDRATE + 0.5)		/* Clocks per one bit (rounded) */

volatile uint8 sRxDone;				/* Ready to read Rx byte */

static volatile uint8 sTxCount;
static volatile uint8 sTxData;
static volatile uint8 sRxData;
static volatile uint8 sRxMask;
static volatile uint8 sRxTemp;

void SWU_Initialize(void);
void SWU_RxByte(uint8 *data);
void SWU_TxByte(uint8 data);
void SWU_RxBuffer(uint8 *buffer, uint8 length);
void SWU_TxBuffer(uint8 *buffer, uint8 length);

/*****************************************************************************/
/* TIMER1 Input Capture Interrupt Handler									 */
/* When Rx pin detects start bit, interrupt handler is called				 */
/* When interrupt handler is called, TCNT1 value is written to ICR1 register */
/*****************************************************************************/
ISR(TIMER1_CAPT_vect)
{
	/* Set OCR1B to read Rx */
	
	OCR1B = ICR1 + (uint16)(BIT_TIME);		/* Output Compare B interrupt will occurs 1.5 BIT_TIME counter(time) later */
	sRxTemp = 0;									/* Clear temporary variable */
	sRxMask = 1;									/* Set bit mask to read first bit */
	TIFR = _BV(OCF1B);								/* Clear Output Compare B interrupt flag */
	
	if(!(SWU_PIN & _BV(SWU_RX)))					/* If RX pin is low, It means start bit */
	{
		TIMSK =  _BV(OCIE1B);			/* Disable input capture(Detecting start bit), Enable Output Compare A(for Tx) / B(for Rx) interrupt */
		//		_BV(OCIE1A) |
		
	}
}

/*****************************************************************************/
/* TIMER1 Output Compare B Interrupt Handler								 */
/* This handler is enabled to read data when system detects start bit		 */
/* sRxMask is true while system reads 8 bits from Rx pin					 */
/*****************************************************************************/
ISR(TIMER1_COMPB_vect)
{
	if(sRxMask)
	{
		if(SWU_PIN & _BV(SWU_RX)) sRxTemp |= sRxMask;		/* if Rx input is high, write 1 to bit */
		sRxMask <<= 1;										/* Shift 1 bit */
		OCR1B += BIT_TIME;									/* Set OCR1B to read next bit */
	}
	
	else
	{
		
		sRxDone = 1;										/* Reading 8 bits is done */
		sRxData = sRxTemp;									/* Store Rx data */
		TIFR = _BV(ICF1);									/* Clear input capture interrupt flag to read new byte */
		TIMSK = _BV(TICIE1);					/* Enable input capture + COMPA interrupt */
		//	 | _BV(OCIE1A)
	}
}

/*****************************************************************************/
/* Software UART Initialization											 */
/* Enable Output Compare A interrupt to send data							 */
/* Enable Input Capture interrupt to receive data			 				 */
/*****************************************************************************/
void SWU_Initialize()
{
	//OCR1A = TCNT1 + 1;						/* Output Compare A interrupt is called immediately after initialization */
	//TCCR1A = _BV(COM1A1) | _BV(COM1A0);		/* Set output mode, set OC1A high(Idle), Timer1 mode 0 */
	TCCR1B = _BV(ICNC1) | _BV(CS10);		/* Input capture noise canceler, falling edge detection, no prescaling */
	TIFR |= _BV(ICF1);						/* Clear capture flag */
	TIMSK |= _BV(TICIE1) ;		/* Enable input capture interrupt(Detect start bit) + Output comapre A match interrupt(Tx) */
	//	| _BV(OCIE1A)
	sTxCount = 0;							/* No Tx Data */
	sRxDone = 0;							/* No Rx Data */
	//SWU_DDR |= _BV(SWU_TX);					/* Set TX pin as output */
	
	sei();									/* Enable global interrupt */
}

/*****************************************************************************/
/* Read Byte																 */
/*****************************************************************************/
void SWU_RxByte(uint8 * data)
{
	while( !sRxDone );
	sRxDone = 0;
	*data = sRxData;
	//USART_TxByte('A');
}

/*****************************************************************************/
/* TIMER1 Output Compare A Interrupt Handler								 */
/* This handler is enabled to send data										 */
/* Be careful, sTxData is inverted data						 				 */
/*****************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	uint8 OutputMode;

	OCR1A += BIT_TIME;								/* Set OCR1A to send next bit */

	if(sTxCount)									/* If there are bits to send */
	{
		sTxCount--;									/* Decrease counter */
		OutputMode = _BV(COM1A1);					/* Set output as low */

		if(sTxCount != 9)							/* If it's data bit */
		{
			if(!(sTxData & 0x01)) OutputMode = _BV(COM1A1) | _BV(COM1A0);		/* If bit is low, set output high (sTxData is inverted) */
			sTxData >>= 1;
		}

		TCCR1A = OutputMode;
	}
}



/*****************************************************************************/
/* Write Byte																 */
/*****************************************************************************/
void SWU_TxByte(uint8 data)
{
	while(sTxCount);
	sTxData = ~data;			/* !!! Write inverted data for stop bit generation */
	sTxCount = 10;				/* Start(1) + data(8) + Stop(1) */
}

/*****************************************************************************/
/* Read Bytes																 */
/*****************************************************************************/
void SWU_RxBuffer(uint8 *buffer, uint8 length)
{
	for(uint8 i = 0; i < length; i++)
	{
		SWU_RxByte(buffer);
		buffer++;
	}
}

/*****************************************************************************/
/* Write Bytes																 */
/*****************************************************************************/

void SWU_TxBuffer(uint8 *buffer, uint8 length)
{
	for(uint8 i = 0; i < length; i++)
	{
		SWU_TxByte(*buffer);
		buffer++;
	}
}

/*****************************************************************************/
/* END															 */
/*****************************************************************************/

#endif /* __8P_SWUART_H_ */
