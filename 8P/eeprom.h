/*
 * EEPROM.h
 * Atmega8
 * Se7S_92
 * Last modification 8-24-2019
 */ 

#include "stdutils.h"

#ifndef _EEPROM_H
#define _EEPROM_H

void EEPROM_write(unsigned int uiAddress, unsigned char ucData) {
	while(EECR & (1 << EEWE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= 1 << EEMWE;
	EECR |= 1 << EEWE;
}


unsigned char EEPROM_read(unsigned int uiAddress) {
    while (EECR & (1 << EEWE));
    EEAR = uiAddress;
    EECR |= (1 << EERE);
    return EEDR;
}


#endif