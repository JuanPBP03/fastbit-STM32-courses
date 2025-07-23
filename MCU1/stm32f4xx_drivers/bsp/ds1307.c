/*
 * ds1307.c
 *
 *  Created on: Jul 23, 2025
 *      Author: JuanP
 */

#include <string.h>
#include <stdint.h>
#include "ds1307.h"

I2C_Handle_t g_ds1307I2CHandle;

static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static void ds1307_write(uint8_t value, uint8_t addr);
static uint8_t ds1307_read(uint8_t addr);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);

uint8_t ds1307_init(void)
{

	ds1307_i2c_pin_config();

	ds1307_i2c_config();

	I2C_PeripheralControl(g_ds1307I2CHandle.instance, ENABLE);

	ds1307_write(0x00, DS1307_ADDR_SEC);

	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7) & 1);
}


void ds1307_set_current_time(RTC_time_t *rtc_time)
{
	uint8_t seconds, hrs;
	seconds = binary_to_bcd(rtc_time->seconds);
	seconds &= ~( 1 << 7);
	ds1307_write(seconds, DS1307_ADDR_SEC);

	ds1307_write(binary_to_bcd(rtc_time->minutes), DS1307_ADDR_MIN);

	hrs = binary_to_bcd(rtc_time->hours);

	if(rtc_time->time_format == TIME_FORMAT_24HRS){
		hrs &= ~(1 << 6);
	}else{
		hrs |= (1 << 6);
		hrs = (rtc_time->time_format  == TIME_FORMAT_12HRS_PM) ? hrs | ( 1 << 5) :  hrs & ~( 1 << 5) ;
	}

	ds1307_write(hrs,DS1307_ADDR_HRS);

}

void ds1307_get_current_time(RTC_time_t *rtc_time)
{

	uint8_t seconds,hrs;

	seconds = ds1307_read(DS1307_ADDR_SEC);

	seconds &= ~( 1 << 7);

	rtc_time->seconds = bcd_to_binary(seconds);
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));

	hrs = ds1307_read(DS1307_ADDR_HRS);
	if(hrs & ( 1 << 6)){
		//12 hr format
		rtc_time->time_format =  !((hrs & ( 1 << 5)) == 0) ;
		hrs &= ~(0x3 << 5);//Clear 6 and 5
	}else{
		//24 hr format
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}

	rtc_time->hours = bcd_to_binary(hrs);
}

void ds1307_set_current_date(RTC_date_t *rtc_date)
{
	ds1307_write(binary_to_bcd(rtc_date->date),DS1307_ADDR_DATE);

	ds1307_write(binary_to_bcd(rtc_date->month),DS1307_ADDR_MONTH);

	ds1307_write(binary_to_bcd(rtc_date->year),DS1307_ADDR_YEAR);

	ds1307_write(binary_to_bcd(rtc_date->day),DS1307_ADDR_DAY);

}



void ds1307_get_current_date(RTC_date_t *rtc_date)
{
	rtc_date->day =  bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));

}


static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda,i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	/*
	 * I2C1_SCL ==> PB6
	 * I2C1_SDA ==> PB7
	 */

	i2c_sda.p_GPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.pinAltFnMode = GPIO_AF_I2C1to3;
	i2c_sda.GPIO_PinConfig.pinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.pinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.pinOType = GPIO_OTYPE_ODRAIN;
	i2c_sda.GPIO_PinConfig.pinPUPDCtrl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;

	GPIO_PinInit(&i2c_sda);


	i2c_scl.p_GPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.pinAltFnMode = 4;
	i2c_scl.GPIO_PinConfig.pinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.pinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.pinOType = GPIO_OTYPE_ODRAIN;
	i2c_scl.GPIO_PinConfig.pinPUPDCtrl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;

	GPIO_PinInit(&i2c_scl);
}

static void ds1307_i2c_config(void)
{
	g_ds1307I2CHandle.instance = DS1307_I2C;
	g_ds1307I2CHandle.config.Ack = I2C_ACK_ENABLE;
	g_ds1307I2CHandle.config.ClockSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2CHandle);

}

static void ds1307_write(uint8_t value, uint8_t addr)
{
	uint8_t tx[2] = {addr, value};

	I2C_MasterTx(&g_ds1307I2CHandle, tx, 2, DS1307_I2C_ADDRESS, I2C_NO_REPEAT);

}

static uint8_t ds1307_read(uint8_t addr)
{

	uint8_t rx;
	I2C_MasterTx(&g_ds1307I2CHandle, &addr, 1, DS1307_I2C_ADDRESS, I2C_REPEAT_START);
	I2C_MasterRx(&g_ds1307I2CHandle, &rx, 1, DS1307_I2C_ADDRESS, I2C_NO_REPEAT);
	return rx;
}

static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m, n;

	if(value<10)
		return value;

	m = (value / 10)<<4;
	n = value % 10;

	return (m | n);

}

static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t) ((value >> 4 ) * 10);
	n =  value & (uint8_t)0x0F;
	return (m+n);
}
