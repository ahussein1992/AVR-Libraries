/*
 * ADC.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for Atmega328 visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#include "stdutils.h"

#ifndef ADC_H_
#define ADC_H_

void ADC_init(void);
uint16 ADC_read(uint8 channel_num);

/* 
 * Description :
 * Function responsible for initialize the ADC driver. 
 */
void ADC_init(void)
{
	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization 
	 */
	ADMUX = 0;
	
	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */ 
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);     
}

/* 
 * Description :
 * Function responsible for read analog data from a certain ADC channel 
 * and convert it to digital using the ADC driver. 
 */
uint16 ADC_read(uint8 channel_num)
{
	channel_num &= 0x07; /* channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */ 
	ADMUX = ADMUX | channel_num; /* choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* wait for conversion to complete ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* clear ADIF by write '1' to it :) */
	return ADC; /* return the data register */
}

#endif /* ADC_H_ */
