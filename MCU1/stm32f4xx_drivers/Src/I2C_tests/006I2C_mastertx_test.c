/*
 * 006I2C_mastertx_test.c
 *
 *  Created on: Jun 19, 2025
 *      Author: JuanP
 */


#include<stdio.h>
#include "stm32f407xx.h"

#define SLAVE_ADDR	0x68


uint8_t some_data[] = "We are testing I2C master Tx\n";


void delay()
{
	for(volatile int i = 0;i<320000;i++);
}

void userButtonInit(void)
{
	GPIO_Handle_t PortA;

	PortA.p_GPIOx = GPIOA;

	GPIO_PinConfig_t ButtonPin = {
		.pinNumber = GPIO_PIN_NO_0,
		.pinMode = GPIO_MODE_IN,
		.pinSpeed = GPIO_SPEED_HIGH,
		.pinPUPDCtrl = GPIO_PUPD_NONE
	};

	PortA.GPIO_PinConfig = ButtonPin;
	GPIO_PinInit(&PortA);
}

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;
	I2CPins.p_GPIOx = GPIOB;

	GPIO_PinConfig_t SCL =
	{
			.pinNumber = 6,
			.pinMode = GPIO_MODE_ALTFN,
			.pinPUPDCtrl = GPIO_PUPD_UP,
			.pinOType = GPIO_OTYPE_ODRAIN,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinAltFnMode = GPIO_AF_I2C1to3
	};
	GPIO_PinConfig_t SDA =
	{
			.pinNumber = 9,
			.pinMode = GPIO_MODE_ALTFN,
			.pinPUPDCtrl = GPIO_PUPD_UP,
			.pinOType = GPIO_OTYPE_ODRAIN,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinAltFnMode = GPIO_AF_I2C1to3
	};
	I2CPins.GPIO_PinConfig = SCL;
	GPIO_PinInit(&I2CPins);
	I2CPins.GPIO_PinConfig = SDA;
	GPIO_PinInit(&I2CPins);
}

I2C_Handle_t hI2C1;


void I2C1_Inits(void)
{
	hI2C1.channel = I2C_CH1;
	hI2C1.instance = I2C1;

	I2C_Config_t config =
	{
			.Ack = I2C_ACK_ENABLE,
			.ClockSpeed = I2C_SPEED_STANDARD,
			.Mode = I2C_MODE_SM,
			.OwnAddr = 0x61,
			.DutyCycle = 0,
	};
	hI2C1.config = config;
	I2C_Init(&hI2C1);


}


int main(void)
{

	I2C1_GPIOInits();
	I2C1_Inits();
	I2C_PeripheralControl(hI2C1.instance, ENABLE);
	userButtonInit();
	while(1){

		while(!GPIO_ReadPin(GPIOA, GPIO_PIN_NO_0));
		delay();

		I2C_MasterTx(&hI2C1, some_data, sizeof(some_data), SLAVE_ADDR);




	}

	return 0;
}
