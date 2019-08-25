/*
 * DS1302.h
 * Atmega_328
 * Se7S_92
 * Last modification 8-24-2019
 */ 

#ifndef DS1302_H_
#define DS1302_H_

#include <avr/sfr_defs.h>

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#define RTC_PORT PORTD
#define RTC_PIN PIND
#define RTC_DDR DDRD
#define RTC_RST PORTD2
#define RTC_SCLK PORTD3
#define RTC_IO PORTD4

// Registers addresses from datasheet 
#define RTC2_SECONDS_READ  0x81
#define RTC2_SECONDS_WRITE 0x80
#define RTC2_MINUTES_READ  0x83
#define RTC2_MINUTES_WRITE 0x82
#define RTC2_HOURS_READ  0x85
#define RTC2_HOURS_WRITE 0x84
#define RTC2_DATE_READ  0x87
#define RTC2_DATE_WRITE 0x86
#define RTC2_MONTH_READ  0x89
#define RTC2_MONTH_WRITE 0x88
#define RTC2_WDAY_READ  0x8B
#define RTC2_WDAY_WRITE 0x8A
#define RTC2_YEAR_READ  0x8D
#define RTC2_YEAR_WRITE 0x8C
#define RTC2_WP_WRITE 0x8E
#define RTC2_CHARGER_WRITE 0x90

#define RTC_CLK_H	RTC_PORT |= _BV(RTC_SCLK)
#define RTC_CLK_L	RTC_PORT &= ~_BV(RTC_SCLK)
#define RTC_IO_H	RTC_PORT |= _BV(RTC_IO)
#define	RTC_IO_L	RTC_PORT &= ~_BV(RTC_IO)
#define RTCRSTH		RTC_PORT |= _BV(RTC_RST)
#define RTCRSTL		RTC_PORT &= ~_BV(RTC_RST)
#define RTC_IO_IN	bit_is_set(RTC_PIN,RTC_IO)
	
typedef struct {
	// time
	uint8 seconds;
	uint8 hours;
	uint8 minutes;

	// date
	uint8 wday;
	uint8 day;
	uint8 month;
	uint8 year;

} rtc2_datetime_t;

typedef rtc2_datetime_t* rtc2_datetime;

volatile rtc2_datetime RTC2_VALUE;
static rtc2_datetime_t rtc2_default = {0};

extern void rtc_init();								// initialise the DS1302
extern void rtc_set_datetime(rtc2_datetime ptr);	// to set your DS1302 you need this command first
extern void rtc_get_date(rtc2_datetime ptr);		// Only get the Date
extern void rtc_get_time(rtc2_datetime ptr);		// Only get the Time
extern void rtc_update(rtc2_datetime ptr);			// get all fields

void write_ds1302_byte(unsigned char cmd)
{
	unsigned char i;
	RTC_DDR |= _BV(RTC_IO)|_BV(RTC_SCLK)|_BV(RTC_RST);
	for(i=0;i<=7;i++)
	{
		if (cmd & 0x01) {RTC_IO_H;} else {RTC_IO_L;}
		cmd >>= 1;
		RTC_CLK_H;	_delay_us(2);
		RTC_CLK_L;	_delay_us(2);
	}
}

void write_ds1302(unsigned char cmd, unsigned char data)
{
	RTCRSTH;
	write_ds1302_byte(cmd);
	write_ds1302_byte(data);
	RTCRSTL;
}

uint8 read_ds1302(unsigned char cmd)
{
	uint8 i,data=0;

	RTCRSTH;
	write_ds1302_byte(cmd);
	RTC_DDR &=~_BV(RTC_IO);
	for(i=0;i<=7;i++)
	{
		data >>= 1;
		if (RTC_IO_IN) {data |= RTC2_SECONDS_WRITE;}

		RTC_CLK_H;	_delay_us(2);
		RTC_CLK_L;	_delay_us(2);
	}
	RTC_DDR |=_BV(RTC_IO);
	RTCRSTL;
	return(data);
}

void rtc_init()
{
	uint8 x;
	RTC2_VALUE = &rtc2_default;
	write_ds1302(RTC2_WP_WRITE,0);
	write_ds1302(RTC2_CHARGER_WRITE,0xa4);
	x = read_ds1302(RTC2_SECONDS_READ);
	if((x & RTC2_SECONDS_WRITE)!=0)
	{
		write_ds1302(RTC2_SECONDS_WRITE,0);
	}
}

uint8 get_bcd(uint8 data)
{
	uint8 nibh;
	uint8 nibl;

	nibh=data/10;
	nibl=data-(nibh*10);

	return((nibh<<4)|nibl);
}

uint8 rm_bcd(uint8 data)
{
	uint8 i;

	i=data;
	data=(i>>4)*10;
	i <<= 4;
	data=data+(i>>4);

	return data;
}

void rtc_set_datetime(rtc2_datetime ptr)
{
	write_ds1302(RTC2_DATE_WRITE,get_bcd(ptr->day));
	write_ds1302(RTC2_MONTH_WRITE,get_bcd(ptr->month));
	write_ds1302(RTC2_YEAR_WRITE,get_bcd(ptr->year));
	write_ds1302(RTC2_WDAY_WRITE,get_bcd(ptr->wday));
	write_ds1302(RTC2_HOURS_WRITE,get_bcd(ptr->hours));
	write_ds1302(RTC2_MINUTES_WRITE,get_bcd(ptr->minutes));
	write_ds1302(RTC2_SECONDS_WRITE,get_bcd(0));
}

void rtc_get_date(rtc2_datetime ptr)
{
	ptr->day = rm_bcd(read_ds1302(RTC2_DATE_READ));
	ptr->month = rm_bcd(read_ds1302(RTC2_MONTH_READ));
	ptr->year = rm_bcd(read_ds1302(RTC2_YEAR_READ));
	ptr->wday = rm_bcd(read_ds1302(RTC2_WDAY_READ));
}

void rtc_get_time(rtc2_datetime ptr)
{
	ptr->hours = rm_bcd(read_ds1302(RTC2_HOURS_READ));
	ptr->minutes = rm_bcd(read_ds1302(RTC2_MINUTES_READ));
	ptr->seconds = rm_bcd(read_ds1302(RTC2_SECONDS_READ));
}

void rtc_update(rtc2_datetime ptr)
{
	ptr->day = rm_bcd(read_ds1302(RTC2_DATE_READ));
	ptr->month = rm_bcd(read_ds1302(RTC2_MONTH_READ));
	ptr->year = rm_bcd(read_ds1302(RTC2_YEAR_READ));
	ptr->wday = rm_bcd(read_ds1302(RTC2_WDAY_READ));
	ptr->hours = rm_bcd(read_ds1302(RTC2_HOURS_READ));
	ptr->minutes = rm_bcd(read_ds1302(RTC2_MINUTES_READ));
	ptr->seconds = rm_bcd(read_ds1302(RTC2_SECONDS_READ));
}

#endif /* DS1302_H_ */