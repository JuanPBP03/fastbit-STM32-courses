/*
 * 002LED_BUTTON.c
 *
 *  Created on: Jun 3, 2025
 *      Author: JuanP
 */


#include "stm32f407xx.h"

void delay(uint32_t x);

int main(void){
	GPIO_Handle_t PortD;
	GPIO_Handle_t PortA;

	PortD.p_GPIOx = GPIOD;
	PortA.p_GPIOx = GPIOA;

	GPIO_PinConfig_t LED4Pin = {
		.GPIO_PinNumber = GPIO_PIN_NO_12,
		.GPIO_PinMode = GPIO_MODE_OUT,
		.GPIO_PinOType = GPIO_OTYPE_PUSHPULL,
		.GPIO_PinSpeed = GPIO_SPEED_HIGH,
		.GPIO_PinPuPdControl = GPIO_PUPD_NONE
	};

	GPIO_PinConfig_t ButtonPin = {
		.GPIO_PinNumber = GPIO_PIN_NO_0,
		.GPIO_PinMode = GPIO_MODE_IT_FT,
		.GPIO_PinSpeed = GPIO_SPEED_HIGH,
		.GPIO_PinPuPdControl = GPIO_PUPD_NONE
	};


	GPIO_periClockControl(GPIO_PORT_D, ENABLE);
	GPIO_periClockControl(GPIO_PORT_A, ENABLE);
	PortD.GPIO_PinConfig = LED4Pin;
	GPIO_PinInit(&PortD);
	PortA.GPIO_PinConfig = ButtonPin;
	GPIO_PinInit(&PortA);
	IRQn_t Button = IRQn_EXTI0;

	GPIO_IRQInterruptConfig(Button, ENABLE);
	GPIO_IRQPriorityConfig(Button, 15);
	while(1);
	return 0;
}

void EXTI0_IRQHandler(void)
{
	GPIO_TogglePin(GPIOD, GPIO_PIN_NO_12);
	GPIO_IRQHandling(0);
}
void delay(uint32_t x)
{
	for(uint32_t i = 0; i < x; i++);
}
