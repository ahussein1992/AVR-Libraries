/*
 * LCD.h
 * Atmega8
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for AVR Microcontrollers visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#include "stdutils.h"

#ifndef _LCD_H
#define _LCD_H

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif
/*_________________________________________________________________________________________*/

/************************************************
	LCD CONNECTIONS
*************************************************/

#define LCD_DATA D	//Port PC0-PC3 are connected to D4-D7

#define LCD_E D 		//Enable OR strobe signal
#define LCD_E_POS	PORTD5	//Position of enable in above port

#define LCD_RS D	
#define LCD_RS_POS 	PORTD7

#define LCD_RW D
#define LCD_RW_POS 	PORTD6

//************************************************
#define LS_BLINK 0B00000001
#define LS_ULINE 0B00000010

/***************************************************
	M A C R O S
***************************************************/
 #define _CONCAT(a,b) a##b
 #define PORT(x) _CONCAT(PORT,x)
 #define PIN(x) _CONCAT(PIN,x)
 #define DDR(x) _CONCAT(DDR,x)
 
#define LCD_DATA_PORT 	PORT(LCD_DATA)
#define LCD_E_PORT 		PORT(LCD_E)
#define LCD_RS_PORT 	PORT(LCD_RS)
#define LCD_RW_PORT 	PORT(LCD_RW)

#define LCD_DATA_DDR 	DDR(LCD_DATA)
#define LCD_E_DDR 		DDR(LCD_E)
#define LCD_RS_DDR 		DDR(LCD_RS)
#define LCD_RW_DDR 		DDR(LCD_RW)

#define LCD_DATA_PIN	PIN(LCD_DATA)

#define SET_E() (LCD_E_PORT|=(1<<LCD_E_POS))
#define SET_RS() (LCD_RS_PORT|=(1<<LCD_RS_POS))
#define SET_RW() (LCD_RW_PORT|=(1<<LCD_RW_POS))

#define CLEAR_E() (LCD_E_PORT&=(~(1<<LCD_E_POS)))
#define CLEAR_RS() (LCD_RS_PORT&=(~(1<<LCD_RS_POS)))
#define CLEAR_RW() (LCD_RW_PORT&=(~(1<<LCD_RW_POS)))

#define LCDCmd(c) (LCDByte(c,0))
#define LCDData(d) (LCDByte(d,1))

#define LCDClear() LCDCmd(0b00000001)
#define LCDHome() LCDCmd(0b00000010);

#define LCDWriteStringXY(x,y,msg) {\
	LCDGotoXY(x,y);\
	LCDWriteString(msg);\
}

#define LCDWriteIntXY(x,y,val,fl) {\
	LCDGotoXY(x,y);\
	LCDWriteInt(val,fl);\
}

/***************************************************/

/***************************************************
			F U N C T I O N S
****************************************************/
void InitLCD(uint8 style);
void LCDWriteString(const char *msg);
void LCDWriteInt(int val,unsigned int field_length);
void LCDGotoXY(uint8 x,uint8 y);

//Low level
void LCDByte(uint8,uint8);

void LCDBusyLoop();

/***************************************************
			F U N C T I O N S     E N D
****************************************************/

void LCDByte(uint8 c,uint8 isdata)
{
	//Sends a byte to the LCD in 4bit mode
	//cmd=0 for data
	//cmd=1 for command
	//NOTE: THIS FUNCTION RETURS ONLY WHEN LCD HAS PROCESSED THE COMMAND

	uint8 hn,ln;			//Nibbles
	uint8 temp;

	hn=c>>4;
	ln=(c & 0x0F);

	if(isdata==0)
		CLEAR_RS();
	else
		SET_RS();

	_delay_us(0.500);		//tAS

	SET_E();

	//Send high nibble

	temp=(LCD_DATA_PORT & 0XF0)|(hn);
	LCD_DATA_PORT=temp;

	_delay_us(1);			//tEH

	//Now data lines are stable pull E low for transmission

	CLEAR_E();

	_delay_us(1);

	//Send the lower nibble
	SET_E();

	temp=(LCD_DATA_PORT & 0XF0)|(ln);

	LCD_DATA_PORT=temp;

	_delay_us(1);			//tEH

	//SEND

	CLEAR_E();

	_delay_us(1);			//tEL

	LCDBusyLoop();
}

void LCDBusyLoop()
{
	//This function waits till lcd is BUSY

	uint8 busy,status=0x00,temp;

	//Change Port to input type because we are reading data
	LCD_DATA_DDR&=0xF0;

	//change LCD mode
	SET_RW();		//Read mode
	CLEAR_RS();		//Read status

	//Let the RW/RS lines stabilize
	_delay_us(0.5);		//tAS

	do
	{

		SET_E();

		//Wait tDA for data to become available
		_delay_us(0.5);

		status=LCD_DATA_PIN;
		status=status<<4;

		_delay_us(0.5);

		//Pull E low
		CLEAR_E();
		_delay_us(1);	//tEL

		SET_E();
		_delay_us(0.5);

		temp=LCD_DATA_PIN;
		temp&=0x0F;

		status=status|temp;

		busy=status & 0b10000000;

		_delay_us(0.5);
		CLEAR_E();
		_delay_us(1);	//tEL
	}while(busy);

	CLEAR_RW();		//write mode
	//Change Port to output
	LCD_DATA_DDR|=0x0F;

}

void InitLCD(uint8 style)
{		
	//style = LS_BLINK|LS_ULINE
	/*****************************************************************
	
	This function Initializes the lcd module
	must be called before calling lcd related functions

	Arguments:
	style = LS_BLINK,LS_ULINE(can be "OR"ed for combination)
	LS_BLINK :The cursor is blinking type
	LS_ULINE :Cursor is "underline" type else "block" type

	*****************************************************************/
	
	//After power on Wait for LCD to Initialize
	_delay_ms(30);
	
	//Set IO Ports
	LCD_DATA_DDR|=(0x0F);
	LCD_E_DDR|=(1<<LCD_E_POS);
	LCD_RS_DDR|=(1<<LCD_RS_POS);
	LCD_RW_DDR|=(1<<LCD_RW_POS);

	LCD_DATA_PORT&=0XF0;
	CLEAR_E();
	CLEAR_RW();
	CLEAR_RS();

	//Set 4-bit mode
	_delay_us(0.3);	//tAS

	SET_E();
	LCD_DATA_PORT|=(0b00000010); //[B] To transfer 0b00100000 i was using LCD_DATA_PORT|=0b00100000
	_delay_us(1);
	CLEAR_E();
	_delay_us(1);
	
	//Wait for LCD to execute the Functionset Command
	LCDBusyLoop();                                    //[B] Forgot this delay

	//Now the LCD is in 4-bit mode

	LCDCmd(0b00001100|style);	//Display On
	LCDCmd(0b00101000);			//function set 4-bit,2 line 5x7 dot format
}

void LCDWriteString(const char *msg)
{
	/*****************************************************************
	
	This function Writes a given string to lcd at the current cursor
	location.

	Arguments:
	msg: a null terminated string to print


	*****************************************************************/
	while(*msg!='\0')
	{
		LCDData(*msg);
		msg++;
	}
}

void LCDWriteInt(int val,unsigned int field_length)
{
	/***************************************************************
	This function writes a integer type value to LCD module

	Arguments:
	1)int val	: Value to print

	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val

	****************************************************************/

	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
		str[i]=val%10;
		val=val/10;
		i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) LCDData('-');
	
	for(i=j;i<5;i++)
	{
		LCDData(48+str[i]);
	}
}

void LCDGotoXY(uint8 x,uint8 y)
{
	if(x<40)
	{
		if(y) x|=0b01000000;
		x|=0b10000000;
		LCDCmd(x);
	}
}
/*_________________________________________________________________________________________*/
#endif





