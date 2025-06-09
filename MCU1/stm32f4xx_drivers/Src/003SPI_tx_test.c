/*
 * 003SPI_tx_test.c
 *
 *  Created on: Jun 9, 2025
 *      Author: JuanP
 */

#include "stm32f407xx.h"

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

	GPIO_periClockControl(GPIOA, ENABLE);
	PORTA.GPIO_PinConfig = SPI1MOSI;
	GPIO_PinInit(&PORTA);
	PORTA.GPIO_PinConfig = SPI1SCK;
	GPIO_PinInit(&PORTA);

	SPI_Handle_t SPI_Test = {.channel = SPI_CH1};

	SPI_Config_t SPI1Cfg = {0};
	SPI1Cfg.Mode = SPI_MODE_MASTER;
	SPI1Cfg.commMode = SPI_COMM_MODE_FULL;
	SPI1Cfg.frameSize = SPI_FRAME_SIZE_8_BIT;
	SPI1Cfg.slaveSelectMode = SPI_SSM_SOFTWARE;
	SPI1Cfg.baudPrescaler = SPI_CLK_16;
	SPI_Test.SPIConfig = SPI1Cfg;

	SPI_periClockEnable(SPI_CH1);

	SPI_Init(&SPI_Test);

	SPI_Enable(SPI_CH1);
	uint8_t message[] = "Hello World";
	while(1){

	SPI_Tx8(SPI_CH1, message, sizeof(message));

	while(SPI_getFlag(SPI_CH1, SPI_FLAG_BUSY));

	}


	return 0;
}
