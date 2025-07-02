/*
 * 007I2C_master_rx_testing.c
 *
 *  Created on: Jun 21, 2025
 *      Author: JuanP
 */



#include<stdio.h>
#include "stm32f407xx.h"


#define SLAVE_ADDR	0x68


uint8_t rcvbuff[32];


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
	hI2C1.instance = I2C1;
	hI2C1.channel = I2C_CH1;

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

uint8_t msgReceived = 0;

int main(void)
{
	uint8_t commandcode;

	uint8_t len;

	printf("Application is running\n");
	I2C1_GPIOInits();
	I2C1_Inits();

	I2C_IRQInterruptConfig(IRQn_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQn_I2C1_ER, ENABLE);

	I2C_PeripheralControl(hI2C1.instance, ENABLE);
	userButtonInit();

	while(1){
		while(!GPIO_ReadPin(GPIOA, GPIO_PIN_NO_0));
		delay();

		commandcode = 0x51;

		while(I2C_MasterTx_IT(&hI2C1, &commandcode, 1, SLAVE_ADDR, I2C_REPEAT_START) != I2C_READY);

		while(I2C_MasterRx_IT(&hI2C1, &len, 1, SLAVE_ADDR, I2C_REPEAT_START) != I2C_READY);
		commandcode = 0x52;

		while(I2C_MasterTx_IT(&hI2C1, &commandcode, 1, SLAVE_ADDR, I2C_REPEAT_START) != I2C_READY);

		while(I2C_MasterRx_IT(&hI2C1, rcvbuff, len, SLAVE_ADDR, I2C_NO_REPEAT) != I2C_READY);
		msgReceived = 0;

		while(msgReceived != 1);

		rcvbuff[len+1] = '\0';

		printf("Data: %s", rcvbuff);
	}

	return 0;
}

void I2C1_EV_IRQHandler(void)
{
	I2C_EventIRQHandler(&hI2C1);
}
void I2C1_ER_IRQHandler(void)
{
	I2C_ErrIRQHandler(&hI2C1);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, I2C_Event_t event)
{
	switch(event){
	case I2C_EV_DATA_RCV:
		printf("EVENT: Tx complete\n");
		break;
	case I2C_EV_DATA_REQ:
		printf("EVENT: Tx complete\n");
		break;
	case I2C_EV_RX_CMPLT:
		printf("EVENT: Rx complete\n");
		msgReceived = 1;
		break;
	case I2C_EV_STOP:
		printf("EVENT: Transaction stopped\n");
		break;
	case I2C_EV_TX_CMPLT:
		printf("EVENT: Tx complete\n");
		break;


	}
}
void I2C_ApplicationErrorCallback(I2C_Handle_t *pI2CHandle, I2C_Error_t error)
{
	switch(error){
	case I2C_ERROR_AF:
		printf("ERROR: Ack Failure\n");
		break;
	case I2C_ERROR_ARLO:
		printf("ERROR: Arbitration lost\n");
		break;
	case I2C_ERROR_BERR:
		printf("ERROR: Bus error\n");
		break;
	case I2C_ERROR_DMA:
		printf("ERROR: DMA\n");
		break;
	case I2C_ERROR_NONE:
		break;
	case I2C_ERROR_OVR:
		printf("ERROR: Overrun/Underrun\n");
		break;
	case I2C_ERROR_TIMEOUT:
		printf("ERROR: Timeout\n");
		break;
	case I2C_ERROR_UNKNOWN:
		printf("ERROR: Unknown\n");
		break;
	}
	I2C_CloseCommunication(&hI2C1);
	I2C_generateStopCondition(hI2C1.instance);
}

