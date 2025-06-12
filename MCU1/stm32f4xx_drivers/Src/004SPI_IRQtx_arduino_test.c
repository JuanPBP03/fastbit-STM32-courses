/*
 * 004SPI_IRQtx_arduino_test.c
 *
 *  Created on: Jun 11, 2025
 *      Author: JuanP
 */

#include "stm32f407xx.h"
#include <string.h>

int main(void){

	GPIO_Handle_t PORTA = { .p_GPIOx = GPIOA};

	GPIO_PinConfig_t SPI1MOSI = {
			.pinAltFnMode = GPIO_AF_SPI1to2_I2S2,
			.pinNumber = GPIO_PIN_NO_7,
			.pinMode = GPIO_MODE_ALTFN,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinPUPDCtrl = GPIO_PUPD_NONE
	};

	GPIO_PinConfig_t SPI1SCK = {
			.pinAltFnMode = GPIO_AF_SPI1to2_I2S2,
			.pinNumber = GPIO_PIN_NO_5,
			.pinMode = GPIO_MODE_ALTFN,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinPUPDCtrl = GPIO_PUPD_NONE
	};

	GPIO_PinConfig_t SPI1CS = {
		.pinAltFnMode = GPIO_AF_SPI1to2_I2S2,
		.pinNumber = GPIO_PIN_NO_4,
		.pinMode = GPIO_MODE_ALTFN,
		.pinSpeed = GPIO_SPEED_HIGH,
		.pinPUPDCtrl = GPIO_PUPD_NONE
	};

	GPIO_PinConfig_t ButtonPin = {
		.pinNumber = GPIO_PIN_NO_0,
		.pinMode = GPIO_MODE_IT_FT,
		.pinSpeed = GPIO_SPEED_HIGH,
		.pinPUPDCtrl = GPIO_PUPD_NONE
	};

	PORTA.GPIO_PinConfig = ButtonPin;
	GPIO_PinInit(&PORTA);
	IRQn_t Button = IRQn_EXTI0;

	GPIO_IRQInterruptConfig(Button, ENABLE);
	GPIO_IRQPriorityConfig(Button, 15);



	GPIO_periClockControl(GPIOA, ENABLE);
	PORTA.GPIO_PinConfig = SPI1MOSI;
	GPIO_PinInit(&PORTA);
	PORTA.GPIO_PinConfig = SPI1SCK;
	GPIO_PinInit(&PORTA);
	PORTA.GPIO_PinConfig = SPI1CS;
	GPIO_PinInit(&PORTA);

	SPI_Handle_t SPI_Test = {.channel = SPI_CH1};

	SPI_Config_t SPI1Cfg = {0};
	SPI1Cfg.Mode = SPI_MODE_MASTER;
	SPI1Cfg.commMode = SPI_COMM_MODE_FULL;
	SPI1Cfg.frameSize = SPI_FRAME_SIZE_8_BIT;
	SPI1Cfg.slaveSelectMode = SPI_SSM_HARDWARE;
	SPI1Cfg.baudPrescaler = SPI_CLK_8;
	SPI_Test.SPIConfig = SPI1Cfg;

	SPI_periClockEnable(SPI_CH1);

	SPI_Init(&SPI_Test);

	char message[] = "I love embedded systems <3";
	SPI_IRQConfig(SPI_CH1, ENABLE);
	SPI_IRQPriority(SPI_CH1, 1);




	while(1){
		if(!(((SPI1->CR1)>>SPI_CR1_SPE_Pos)&0))
			SPI_TxIRQ(SPI_CH1, (uint16_t*)message, strlen(message));
	}



	return 0;
}

void SPI1_IRQHandler(void){
	SPI_TxStartFrame(SPI_CH1);
	SPI_IRQHandler(SPI_CH1);
	if(!SPI_isBusy(SPI_CH1)){
		while(SPI_getFlag(SPI_CH1, SPI_FLAG_BUSY));
		SPI_Disable(SPI_CH1);
	}
}
void EXTI0_IRQHandler(void)
{
	SPI_Enable(SPI_CH1);
	GPIO_IRQHandling(0);
}
