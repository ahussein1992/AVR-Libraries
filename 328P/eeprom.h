/*
 * EEPROM.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 */ 

#ifndef _EEPROM_H
#define _EEPROM_H

void EEPROM_write(unsigned int uiAddress, unsigned char ucData) {
	while(EECR & (1 << EEPE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= 1 << EEMPE;
	EECR |= 1 << EEPE;
}


unsigned char EEPROM_read(unsigned int uiAddress) {
    while (EECR & (1 << EEPE));
    EEAR = uiAddress;
    EECR |= (1 << EERE);
    return EEDR;
}

#endif