/*
 * 005SPI_TxRx_test.c
 *
 *  Created on: Jun 11, 2025
 *      Author: JuanP
 */




#include "stm32f407xx.h"
#include <string.h>
#include <stdbool.h>
int main(void){

	GPIO_Handle_t PORTA = { .p_GPIOx = GPIOA};

	GPIO_PinConfig_t SPI1MOSI = {
			.pinAltFnMode = GPIO_AF_SPI1to2_I2S2,
			.pinNumber = GPIO_PIN_NO_7,
			.pinMode = GPIO_MODE_ALTFN,
			.pinSpeed = GPIO_SPEED_HIGH,
			.pinPUPDCtrl = GPIO_PUPD_NONE
	};
	GPIO_PinConfig_t SPI1MISO = {
			.pinAltFnMode = GPIO_AF_SPI1to2_I2S2,
			.pinNumber = GPIO_PIN_NO_6,
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



	PORTA.GPIO_PinConfig = SPI1MOSI;
	GPIO_PinInit(&PORTA);
	PORTA.GPIO_PinConfig = SPI1MISO;
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
	SPI1Cfg.baudPrescaler = SPI_CLK_4;
	SPI_Test.SPIConfig = SPI1Cfg;

	SPI_periClockEnable(SPI_CH1);

	SPI_Init(&SPI_Test);

	uint8_t init[] = {0x50,0xFF};
	uint8_t command[]={0x09,0x01};
	uint8_t rx[2] = {0, 0};
	SPI_IRQPriority(SPI_CH1, 1);



	while(1){
		SPI_TxRxIRQ(SPI_CH1, init, rx, sizeof(init));
		while(SPI_isBusy(SPI_CH1));

		if(rx[1]==0xf5){
			SPI_TxRxIRQ(SPI_CH1, command, rx ,sizeof(command));
			SPI_Enable(SPI_CH1);
			command[1] ^= 1;
			rx[1]=0;
			while(SPI_isBusy(SPI_CH1));

		}

		SPI_Disable(SPI_CH1);

	}

	return 0;
}


void SPI1_IRQHandler(void){


SPI_RxIRQHandler(SPI_CH1);


SPI_TxIRQHandler(SPI_CH1);


}


void EXTI0_IRQHandler(void)
{
	GPIO_IRQHandling(0);
	SPI_Enable(SPI_CH1);

}
