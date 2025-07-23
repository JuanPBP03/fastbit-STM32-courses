/*
 * USART_senddata_test.c
 *
 *  Created on: Jul 12, 2025
 *      Author: JuanP
 */

#include <stdio.h>
#include "stm32f407xx.h"

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


void USART1_GPIOInits(void)
{
	GPIO_Handle_t USARTPins;
	USARTPins.p_GPIOx = GPIOA;

	GPIO_PinConfig_t TxUSART =
	{
			.pinNumber = GPIO_PIN_NO_2,
			.pinMode = GPIO_MODE_ALTFN,
			.pinPUPDCtrl = GPIO_PUPD_NONE,
			.pinOType = GPIO_OTYPE_PUSHPULL,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinAltFnMode = GPIO_AF_USART1to3_I2S3ext
	};
	GPIO_PinConfig_t RxUSART =
	{
			.pinNumber = GPIO_PIN_NO_3,
			.pinMode = GPIO_MODE_ALTFN,
			.pinPUPDCtrl = GPIO_PUPD_NONE,
			.pinOType = GPIO_OTYPE_PUSHPULL,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinAltFnMode = GPIO_AF_USART1to3_I2S3ext
	};

	USARTPins.GPIO_PinConfig = TxUSART;
	GPIO_PinInit(&USARTPins);
	USARTPins.GPIO_PinConfig = RxUSART;
	GPIO_PinInit(&USARTPins);
}

USART_Handle_t hUSART1;

void USART1_Inits(void)
{
	hUSART1.instance = USART2;
	hUSART1.ch = USART_CH2;
	hUSART1.Config.Baud = 115200;
	hUSART1.Config.HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	hUSART1.Config.Mode = USART_MODE_TXRX;
	hUSART1.Config.StopBits = USART_STOPBITS_1;
	hUSART1.Config.WordLength = USART_WORDLEN_8BITS;
	hUSART1.Config.ParityControl = USART_PARITY_DISABLE;

	USART_Init(&hUSART1);
}

uint8_t msg[] = "UART Tx testing...";

int main(void){

	USART1_GPIOInits();
	USART1_Inits();
	userButtonInit();

	USART_PeripheralControl(hUSART1.instance, ENABLE);
	while(1){

		while(!GPIO_ReadPin(GPIOA, GPIO_PIN_NO_0));
		delay();

		USART_SendData(&hUSART1, msg, sizeof(msg));


	}
	return 0;
}
