/*
 * stm32f4xx_usart_driver.c
 *
 *  Created on: Jul 9, 2025
 *      Author: JuanP
 */

#include "stm32f407xx.h"


/*********************************************************************
 * @fn      		  - USART_SetBaudRate
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -  Resolve all the TODOs

 */
void USART_SetBaudRate(USART_RegDef_t *instance, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

	uint32_t tempreg=0;

	//Get the value of APB bus clock in to the variable PCLKx
	if(instance == USART1 || instance == USART6)
	{
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Freq();
	}else
	{
	   PCLKx = RCC_GetPCLK1Freq();
	}

	//Check for OVER8 configuration bit
	if(instance->CR1 & (1 << TODO))
	{
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	}else
	{
	   //over sampling by 16
	   TODO
	}

	//Calculate the Mantissa part
	M_part = TODO/100;

	//Place the Mantissa part in appropriate bit position . refer USART_BRR
	tempreg |= M_part << 4;

	//Extract the fraction part
	F_part = (usartdiv - (TODO * 100));

	//Calculate the final fractional
	if(instance->CR1 & ( 1 << USART_CR1_OVER8))
	{
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * TODO)+ 50) / 100)& ((uint8_t)0x07);

	}else
	{
	   //over sampling by 16
	   F_part = ((( F_part * TODO)+ 50) / 100) & ((uint8_t)0x0F);

	}

	//Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	//copy the value of tempreg in to BRR register
	instance->TODO = tempreg;
}


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

	//Implement the code to enable the Receiver bit field
	tempreg|= (hUSART->Config.Mode << 2);


    //Implement the code to configure the Word length configuration item
	tempreg |= hUSART->Config.WordLength << 12;


    //Configuration of parity control bit fields
	if ( hUSART->Config.ParityControl == USART_PARITY_EN_EVEN)
	{
		//Implement the code to enale the parity control
		tempreg |= USART_CR1_PCE;

		//Implement the code to enable EVEN parity
		//Not required because by default EVEN parity will be selected once you enable the parity control

	}else if (hUSART->Config.ParityControl == USART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control
	    tempreg |= USART_CR1_PCE;

	    //Implement the code to enable ODD parity
	    tempreg |= USART_CR1_PS;

	}

   //Program the CR1 register
	hUSART->instance->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of stop bits inserted during USART frame transmission
	tempreg |= hUSART->Config.StopBits << USART_CR2_STOP_POS;

	//Program the CR2 register
	hUSART->instance->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to enable CTS flow control
		tempreg |= USART_CR3_CTSE;


	}else if (hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to enable RTS flow control
		tempreg |= USART_CR3_RTSE;

	}else if (hUSART->Config.HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both CTS and RTS Flow control
		tempreg |= (USART_CR3_CTSE | USART_CR3_RTSE);
	}


	hUSART->instance->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here

}

void USART_DeInit(USART_Handle_t *hUSART){

	if(((hUSART->ch)>5)||((hUSART->ch)<0)) return;

	if((hUSART->ch) == 0 || (hUSART->ch) == 5){
		RCC->APB2RSTR |= (1UL<<(4+((hUSART->ch)&1)));
		RCC->APB2RSTR &= ~(1UL<<(4+((hUSART->ch)&1)));
	}
	else{
		RCC->APB1RSTR |= ((1UL)<<(16+(hUSART->ch)));
		RCC->APB1RSTR &= ~((1UL)<<(16+(hUSART->ch)));

	}
}


/*
 * Data Send and Receive
 */


/*********************************************************************
 * @fn      		  - USART_SendData
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
void USART_SendData(USART_Handle_t *hUSART, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		while(!USART_GetFlagStatus(hUSART->instance, USART_FLAG_TXE));

         //Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(hUSART->Config.WordLength == USART_WORDLEN_9BITS)
		{
			//if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			hUSART->instance->DR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(hUSART->Config.ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			hUSART->instance->DR = (*pTxBuffer  & (uint8_t)0xFF);
			pTxBuffer++;

		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(hUSART->instance,USART_FLAG_TC));
}


/*********************************************************************
 * @fn      		  - USART_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */

void USART_ReceiveData(USART_Handle_t *hUSART, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while(!USART_GetFlagStatus(hUSART->instance, USART_FLAG_RXNE))

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(hUSART->Config.WordLength == USART_WORDLEN_9BITS)
		{
			//We are going to receive 9bit data in a frame

			//check are we using USART_ParityControl control or not
			if(hUSART->Config.ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used. so, all 9bits will be of user data

				//read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (hUSART->instance->DR  & (uint16_t)0x1FF);

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;

			}
			else
			{
				//Parity is used, so, 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (hUSART->instance->DR  & (uint8_t)0xFF);

				 //Increment the pRxBuffer
				pRxBuffer++;


			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//check are we using USART_ParityControl control or not
			if(hUSART->Config.ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				 *pRxBuffer = hUSART->instance->DR;
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t)(hUSART->instance->DR & (uint8_t)0x7F);

			}

			//increment the pRxBuffer
			pRxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - USART_SendDataWithIT
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
uint8_t USART_SendDataIT(USART_Handle_t *hUSART,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = hUSART->TxBusyState;

	if(txstate != USART_BUSY_IN_TX)
	{
		hUSART->TxLen = Len;
		hUSART->pTxBuffer = pTxBuffer;
		hUSART->TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		hUSART->instance->CR1 |= USART_CR1_TXEIE;


		//Implement the code to enable interrupt for TC
		hUSART->instance->CR1 |= USART_CR1_TCIE;



	}

	return txstate;

}


/*********************************************************************
 * @fn      		  - USART_ReceiveDataIT
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
uint8_t USART_ReceiveDataIT(USART_Handle_t *hUSART,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = hUSART->RxBusyState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		hUSART->RxLen = Len;
		hUSART->pRxBuffer = pRxBuffer;
		hUSART->RxBusyState = USART_BUSY_IN_RX;

		//Implement the code to enable interrupt for RXNE
		hUSART->instance->CR1 |= USART_CR1_RXNEIE;


	}

	return rxstate;

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
uint8_t USART_GetFlagStatus(USART_RegDef_t *instance, uint8_t StatusFlagName)
{
    if(instance->SR & StatusFlagName)
    {
    	return SET;
    }

   return RESET;
}
void USART_ClearFlag(USART_RegDef_t *instance, uint16_t StatusFlagName){

}

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *hUSART, uint8_t AppEv);
