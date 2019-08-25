/*
 * I2C.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for AVR Microcontrollers visit https://github.com/ahussein1992/AVR-Libraries
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "stdutils.h"

#define My_Address       0x02 // my address 
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start 
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave

void TWI_Init(void);
void TWI_Start(void);
void TWI_Stop(void);
void TWI_Write(unsigned char data);
unsigned char TWI_Read_With_ACK(void); //read with send Ack
unsigned char TWI_Read_With_NACK(void); //read without send Ack
unsigned char TWI_Get_Status(void);

void TWI_Init(void)
{
    // Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz
    TWBR = 0x02;
	TWSR = 0x00;
	
    // Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
    // General Call Recognition: Off
    TWAR = My_Address; // my address = 0x02 :) 
	
    TWCR = (1<<TWEN); //enable TWI
}

void TWI_Start(void)
{
    /* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // Wait for TWINT flag set in TWCR Register (start bit is send successfully) 
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_Stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_Write(uint8 data)
{
    // Put data On TWI data Register
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);
    // Wait for TWINT flag set in TWCR Register(data is send successfully) 
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_Read_With_ACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    // Wait for TWINT flag set in TWCR Register (data received successfully) 
    while(BIT_IS_CLEAR(TWCR,TWINT));
    // Read Data
    return TWDR;
}

uint8 TWI_Read_With_NACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    // Wait for TWINT flag set in TWCR Register (data received successfully) 
    while(BIT_IS_CLEAR(TWCR,TWINT));
    // Read Data
    return TWDR;
}

uint8 TWI_Get_Status(void)
{
    uint8 status;
    status = TWSR & 0xF8; //masking to eliminate first 3 bits 
    return status;
}

#endif /* I2C_H_ */
