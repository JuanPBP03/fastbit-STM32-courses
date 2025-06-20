/*
 * 001LED_TOGGLE.c
 *
 *  Created on: Jun 3, 2025
 *      Author: JuanP
 */

#include "stm32f407xx.h"

void delay(uint32_t x);

int main(void){
	GPIO_Handle_t PortD;

	PortD.p_GPIOx = GPIOD;
	GPIO_PinConfig_t LED4Pin = {
		.GPIO_PinNumber = GPIO_PIN_NO_12,
		.GPIO_PinMode = GPIO_MODE_OUT,
		.GPIO_PinOType = GPIO_OTYPE_PUSHPULL,
		.GPIO_PinSpeed = GPIO_SPEED_HIGH,
		.GPIO_PinPuPdControl = GPIO_PUPD_NONE
	};

	GPIO_PinConfig_t LED3Pin = {
		.GPIO_PinNumber = GPIO_PIN_NO_13,
		.GPIO_PinMode = GPIO_MODE_OUT,
		.GPIO_PinOType = GPIO_OTYPE_PUSHPULL,
		.GPIO_PinSpeed = GPIO_SPEED_HIGH,
		.GPIO_PinPuPdControl = GPIO_PUPD_NONE
	};


	GPIO_periClockControl(GPIOD, ENABLE);
	PortD.GPIO_PinConfig = LED4Pin;
	GPIO_PinInit(&PortD);
	PortD.GPIO_PinConfig = LED3Pin;
	GPIO_PinInit(&PortD);
	while(1){
		GPIO_TogglePin(GPIOD, GPIO_PIN_NO_12);
		delay(500000);
		GPIO_TogglePin(GPIOD, GPIO_PIN_NO_13);

	}


	return 0;
}

void delay(uint32_t x)
{
	for(uint32_t i = 0; i < x; i++);
}
