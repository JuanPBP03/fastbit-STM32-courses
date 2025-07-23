/*
 * USART_rxtest.c
 *
 *  Created on: Jul 23, 2025
 *      Author: JuanP
 */


#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"



//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

static USART_Handle_t usart2_handle;


//This flag indicates reception completion
static uint8_t rxCmplt = RESET;

static uint8_t g_data = 0;

extern void initialise_monitor_handles();

void USART2_Init(void)
{
	usart2_handle.instance = USART2;
	usart2_handle.ch = USART_CH2;
	usart2_handle.Config.Baud = USART_STD_BAUD_115200;
	usart2_handle.Config.HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.Config.Mode = USART_MODE_TXRX;
	usart2_handle.Config.StopBits = USART_STOPBITS_1;
	usart2_handle.Config.WordLength = USART_WORDLEN_8BITS;
	usart2_handle.Config.ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

void 	USART2_GPIOInit(void)
{
	GPIO_Handle_t usart_gpios;

	usart_gpios.p_GPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.pinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.pinOType = GPIO_OTYPE_PUSHPULL;
	usart_gpios.GPIO_PinConfig.pinPUPDCtrl = GPIO_PUPD_UP;
	usart_gpios.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;
	usart_gpios.GPIO_PinConfig.pinAltFnMode = GPIO_AF_USART1to3_I2S3ext;

	usart_gpios.GPIO_PinConfig.pinNumber  = GPIO_PIN_NO_2;
	GPIO_PinInit(&usart_gpios);

	usart_gpios.GPIO_PinConfig.pinNumber = GPIO_PIN_NO_3;
	GPIO_PinInit(&usart_gpios);

}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.p_GPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.pinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.pinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;
	GPIOBtn.GPIO_PinConfig.pinPUPDCtrl = GPIO_PUPD_NONE;

	GPIO_PinInit(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.p_GPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.pinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.pinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.pinOType = GPIO_OTYPE_ODRAIN;
	GpioLed.GPIO_PinConfig.pinPUPDCtrl = GPIO_PUPD_NONE;

	GPIO_periClockControl(GPIOD,ENABLE);

	GPIO_PinInit(&GpioLed);

}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}
int main(void)
{
	uint32_t cnt = 0;


	initialise_monitor_handles();

	USART2_GPIOInit();
    USART2_Init();

    USART_IRQInterruptConfig(IRQn_USART2,ENABLE);

    USART_PeripheralControl(USART2,ENABLE);

    printf("Application is running\n");

    //do forever
    while(1)
    {
		//wait till button is pressed
		while( ! GPIO_ReadPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		// Next message index ; make sure that cnt value doesn't cross 2
		cnt = cnt % 3;

		//First lets enable the reception in interrupt mode
		//this code enables the receive interrupt
		while (USART_ReceiveDataIT(&usart2_handle,(uint8_t*)rx_buf,strlen(msg[cnt])) != USART_READY );

		//Send the msg indexed by cnt in blocking mode
    	USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

    	printf("Transmitted : %s\n",msg[cnt]);


    	//Now lets wait until all the bytes are received from the arduino .
    	//When all the bytes are received rxCmplt will be SET in application callback
    	while(rxCmplt != SET);

    	//just make sure that last byte should be null otherwise %s fails while printing
    	rx_buf[strlen(msg[cnt])+ 1] = '\0';

    	//Print what we received from the arduino
    	printf("Received    : %s\n",rx_buf);

    	//invalidate the flag
    	rxCmplt = RESET;

    	//move on to next message indexed in msg[]
    	cnt ++;
    }


	return 0;
}


void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}





void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
			rxCmplt = SET;

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
   }
}
