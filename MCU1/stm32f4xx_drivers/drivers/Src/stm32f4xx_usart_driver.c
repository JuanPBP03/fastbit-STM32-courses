/*
 * stm32f4xx_usart_driver.c
 *
 *  Created on: Jul 9, 2025
 *      Author: JuanP
 */

#include "stm32f407xx.h"

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_Channel_t ch, uint8_t ENorDI){
	if((ch>5)||(ch<0)) return;

	if(ENorDI==ENABLE){
		if(ch == 0 || ch == 5)
			RCC->APB2ENR |= (1UL<<(4+(ch&1)));
		else
			RCC->APB1ENR |= (1UL)<<(16+ch);
	}
	else if(ENorDI==DISABLE){
		if(ch == 0 || ch == 5)
			RCC->APB2ENR &= ~(1UL<<(4+(ch&1)));
		else
			RCC->APB1ENR &= ~(1UL)<<(16+ch);
	}
}


/*
 * Init and De-init
 */
/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
void USART_Init(USART_Handle_t *hUSART)
{

	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given USART peripheral
	 USART_PeriClockControl(hUSART->ch, ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( hUSART->Config.Mode == USART_MODE_ONLY_RX)
	{
		//Implement the code to enable the Receiver bit field
		//tempreg|= (1 << TODO);
	}else if (hUSART->Config.Mode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field
		//tempreg |= ( 1 << TODO );

	}else if (hUSART->Config.Mode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields
		//tempreg |= ( ( 1 << TODO) | ( 1 << TODO) );
	}

    //Implement the code to configure the Word length configuration item
	//tempreg |= hUSART->Config.WordLength << TODO ;


    //Configuration of parity control bit fields
	if ( hUSART->Config.ParityControl == USART_PARITY_EN_EVEN)
	{
		//Implement the code to enale the parity control
		//tempreg |= ( 1 << TODO);

		//Implement the code to enable EVEN parity
		//Not required because by default EVEN parity will be selected once you enable the parity control

	}else if (hUSART->Config.ParityControl == USART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control
	    tempreg |= USART_CR1_PCE;

	    //Implement the code to enable ODD parity
	  //  tempreg |= ( 1 << TODO);

	}

   //Program the CR1 register
	//hUSART->instance->CR1 = TODO;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of stop bits inserted during USART frame transmission
	//tempreg |= hUSART->Config.StopBits << TODO;

	//Program the CR2 register
	hUSART->instance->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to enable CTS flow control
		//tempreg |= ( 1 << TODO);


	}else if (hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to enable RTS flow control
		//tempreg |= TODO;

	}else if (hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both CTS and RTS Flow control
		//TODO
	}


	//hUSART->instance->TODO = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here

}

void USART_DeInit(USART_RegDef_t *instance){

}


/*
 * Data Send and Receive
 */
void USART_SendData(USART_RegDef_t *instance,uint8_t *pTxBuffer, uint32_t Len){

}
void USART_ReceiveData(USART_RegDef_t *instance, uint8_t *pRxBuffer, uint32_t Len){

}
uint8_t USART_SendDataIT(USART_Handle_t *hUSART,uint8_t *pTxBuffer, uint32_t Len){
	return 0;
}
uint8_t USART_ReceiveDataIT(USART_Handle_t *hUSART, uint8_t *pRxBuffer, uint32_t Len){
	return 0;
}

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(IRQn_t IRQn, uint8_t ENorDI)
{

	if((int32_t)IRQn >= 0){
		if(ENorDI == ENABLE){
		NVIC->ISER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
		}
		else
		{
		NVIC->ICER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
		}
	}

}

void USART_IRQPriorityConfig(IRQn_t IRQn, uint8_t priority)
{
	NVIC->IPR[((uint32_t)IRQn) >> 2U] &= ~(0xFFU << (((uint32_t)(IRQn) & 0x3U)*8));
	NVIC->IPR[((uint32_t)IRQn) >> 2U] |= (uint32_t)(((priority&0x0F)<<__NVIC_PRIO_BITS)<<(((uint32_t)(IRQn) & 0x3U)*8));
}

void USART_IRQHandling(USART_Handle_t *hUSART){

}

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *instance, uint8_t EnOrDi){

}
uint8_t USART_GetFlagStatus(USART_RegDef_t *instance , uint32_t FlagName){
	return 0;
}
void USART_ClearFlag(USART_RegDef_t *instance, uint16_t StatusFlagName){

}

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *hUSART, uint8_t AppEv);
