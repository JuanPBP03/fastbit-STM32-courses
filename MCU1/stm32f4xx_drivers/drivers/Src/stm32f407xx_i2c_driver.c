/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Jun 10, 2025
 *      Author: juanp
 */


#include "stm32f407xx_i2c_driver.h"
#include "stddef.h"

#ifdef USE_SINGLETON_HANDLES
struct I2C_Handle{
	I2C_RegDef_t *instance;
	I2C_Channel_t channel;
	uint32_t ClockSpeed;
	uint8_t DutyCycle;
	uint8_t OwnAddr;
	uint8_t Ack;
};

static I2C_Handle_t *const hI2C[I2C_CHANNELS] =
{
		{I2C1,			I2C_CH1,		I2C_SPEED_SLOW,			I2C_FM_DUTY_2,			0x00,			I2C_ACK_ENABLE},
		{I2C2,			I2C_CH2,		I2C_SPEED_SLOW,			I2C_FM_DUTY_2,			0x01,			I2C_ACK_ENABLE},
		{I2C3,			I2C_CH3,		I2C_SPEED_SLOW,			I2C_FM_DUTY_2,			0x02,			I2C_ACK_ENABLE},

};

#endif

#define __I2C_DISABLEACK(__REGDEF__)		(__REGDEF__)->CR1 &= ~I2C_CR1_ACK
#define __I2C_ENABLEACK(__REGDEF__)		(__REGDEF__)->CR1 |= I2C_CR1_ACK

#define __I2C_DISABLEPOS(__REGDEF__)		(__REGDEF__)->CR1 &= ~I2C_CR1_POS
#define __I2C_ENABLEPOS(__REGDEF__)		(__REGDEF__)->CR1 |= I2C_CR1_POS

static inline void I2C_generateStartCondition(I2C_RegDef_t *instance);
static void I2C_executeAddressPhase(I2C_RegDef_t *instance, uint8_t addr, uint8_t rw);
static void I2C_clearADDRFlag(I2C_Handle_t *hI2C);
inline void I2C_generateStopCondition(I2C_RegDef_t *instance);
void I2C_CloseCommunication(I2C_Handle_t *hI2C);



void I2C_generateStopCondition(I2C_RegDef_t *instance)
{
	instance->CR1 |= I2C_CR1_STOP;

}

static void I2C_clearADDRFlag(I2C_Handle_t *hI2C)
{
	uint32_t dummy;
	if(hI2C->instance->SR2 & I2C_SR2_MSL)
	{
		if(hI2C->State == I2C_BUSY_IN_RX)
		{
			if(hI2C->RxSize == 1)
			{
				__I2C_DISABLEACK(hI2C->instance);

			}else if(hI2C->RxSize==2)
			{
				__I2C_DISABLEACK(hI2C->instance);
				__I2C_ENABLEPOS(hI2C->instance);
			}

		}

	}

	dummy = hI2C->instance->SR1;
	dummy = hI2C->instance->SR2;
	(void)dummy;
}

#define READ	1
#define WRITE	0
static void I2C_executeAddressPhase(I2C_RegDef_t *instance, uint8_t addr, uint8_t rw)
{
	addr = addr<<1;
	if(rw == READ)
		addr |= 1;
	else
		addr &= ~(1);
	instance->DR = addr;
}

static inline void I2C_generateStartCondition(I2C_RegDef_t *instance)
{
	instance->CR1 |= I2C_CR1_START;
}


void I2C_periClockCtrl(I2C_Channel_t ch, uint8_t ENorDI)
{

	if((ch>2)||(ch<0)) return;
	if(ENorDI==ENABLE)
		RCC->APB1ENR |= (1UL<<(21+ch));
	else if(ENorDI==DISABLE)
		RCC->APB1ENR &= ~(1UL<<(21+ch));
}

void I2C_Init(I2C_Handle_t *hI2C)
{
	I2C_periClockCtrl(hI2C->channel, ENABLE);


	uint16_t ccr;
	uint32_t PCLK1 = RCC_GetPCLK1Freq();
	if(hI2C->config.ClockSpeed <= I2C_SPEED_STANDARD)
	{
		hI2C->instance->CCR &= ~I2C_CCR_FS;
		ccr = PCLK1 / (2*hI2C->config.ClockSpeed);
		ccr = (ccr < 4) ? 4 : ccr;
		hI2C->instance->TRISE &= ~0x3FUL;
		hI2C->instance->TRISE |= (0x3FUL)&((RCC_GetPCLK1Freq()/1000000)+1);

	}
	else {

		hI2C->instance->CCR |= I2C_CCR_FS;
		if(hI2C->config.DutyCycle == I2C_FM_DUTY_2){
			hI2C->instance->CCR &= ~I2C_CCR_DUTY;
			ccr = PCLK1 / (3*hI2C->config.ClockSpeed);
		}else{
			hI2C->instance->CCR |= I2C_CCR_DUTY;
			ccr = PCLK1 / (25*hI2C->config.ClockSpeed);
		}
		hI2C->instance->TRISE &= ~0x3FUL;
		hI2C->instance->TRISE |= (0x3FUL)&( ( (RCC_GetPCLK1Freq()*300) / 1000000000)+1);
	}
	hI2C->instance->CCR &= ~I2C_CCR_CCR;
	hI2C->instance->CCR |= (ccr & I2C_CCR_CCR_Msk);
	hI2C->instance->CR2 &= ~I2C_CR2_FREQ;
	hI2C->instance->CR2 |= (PCLK1 & I2C_CR2_FREQ);
	hI2C->instance->OAR1 |= hI2C->config.OwnAddr<<1;
	hI2C->instance->OAR1 |= (1UL<<14);
	if (hI2C->config.Ack == I2C_ACK_ENABLE)
		hI2C->instance->CR1 |= I2C_CR1_ACK;
	else
		hI2C->instance->CR1 &= I2C_CR1_ACK;
}
void I2C_DeInit(I2C_RegDef_t *instance);

void I2C_MasterTx(I2C_Handle_t *hI2C, uint8_t *pTxBuffer, uint32_t len, uint8_t addr, uint8_t repeatedStart)
{
	I2C_generateStartCondition(hI2C->instance);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_SB));

	I2C_executeAddressPhase(hI2C->instance, addr, WRITE);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_ADDR));
	I2C_clearADDRFlag(hI2C);
	while(len>0){
		while(!I2C_getFlag(hI2C->instance, I2C_FLAG_TXE));
		hI2C->instance->DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_TXE));
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_BTF));

	//	Generate stop condition ONLY IF no repeated starts
	if(repeatedStart == I2C_NO_REPEAT)
		I2C_generateStopCondition(hI2C->instance);

}

void I2C_MasterRx(I2C_Handle_t *hI2C, uint8_t *pRxBuffer, uint32_t len, uint8_t addr, uint8_t repeatedStart)
{
	I2C_generateStartCondition(hI2C->instance);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_SB));

	I2C_executeAddressPhase(hI2C->instance, addr, READ);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_ADDR));

	if(len == 1)
	{
		__I2C_DISABLEACK(hI2C->instance);
		I2C_clearADDRFlag(hI2C);
		while(!I2C_getFlag(hI2C->instance, I2C_FLAG_RXNE));
		//	Generate stop condition ONLY IF no repeated starts
		if(repeatedStart == I2C_NO_REPEAT)
			I2C_generateStopCondition(hI2C->instance);
		*(pRxBuffer) = hI2C->instance->DR;

	}



	if(len>1)
	{
		if(len==2)
		{
			__I2C_DISABLEACK(hI2C->instance);
			__I2C_ENABLEPOS(hI2C->instance);
		}
		I2C_clearADDRFlag(hI2C);

		while(len>=3)
		{
			if(len==3)
			{
				while(!I2C_getFlag(hI2C->instance, I2C_FLAG_BTF));
				__I2C_DISABLEACK(hI2C->instance);

			}

			while(!I2C_getFlag(hI2C->instance, I2C_FLAG_RXNE));
			*pRxBuffer++ = hI2C->instance->DR;
			len--;
		}

		while(!I2C_getFlag(hI2C->instance, I2C_FLAG_BTF));
		//	Generate stop condition ONLY IF no repeated starts
		if(repeatedStart == I2C_NO_REPEAT)
			I2C_generateStopCondition(hI2C->instance);
		*pRxBuffer++ = hI2C->instance->DR;
		*pRxBuffer = hI2C->instance->DR;

	}
	__I2C_DISABLEPOS(hI2C->instance);
	if(hI2C->config.Ack == I2C_ACK_ENABLE)
		__I2C_ENABLEACK(hI2C->instance);


}


I2C_State_t I2C_MasterTx_IT(I2C_Handle_t *hI2C, uint8_t *pTxBuffer, uint32_t len, uint8_t addr, uint8_t repeatedStart)
{
	uint8_t busystate = hI2C->State;

		if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
		{
			hI2C->pTxBuffer = pTxBuffer;
			hI2C->TxLen = len;
			hI2C->State = I2C_BUSY_IN_TX;
			hI2C->DevAddr = addr;
			hI2C->repeat = repeatedStart;

			//Implement code to Generate START Condition
			I2C_generateStartCondition(hI2C->instance);

			//Implement the code to enable ITBUFEN Control Bit
			hI2C->instance->CR2 |= I2C_CR2_ITBUFEN;

			//Implement the code to enable ITEVFEN Control Bit
			hI2C->instance->CR2 |= I2C_CR2_ITEVTEN;


			//Implement the code to enable ITERREN Control Bit
			hI2C->instance->CR2 |= I2C_CR2_ITERREN;


		}

		return busystate;
}
I2C_State_t I2C_MasterRx_IT(I2C_Handle_t *hI2C, uint8_t *pRxBuffer, uint32_t len, uint8_t addr, uint8_t repeatedStart)
{
	uint8_t busystate = hI2C->State;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		hI2C->pRxBuffer = pRxBuffer;
		hI2C->RxLen = len;
		hI2C->State = I2C_BUSY_IN_RX;
		hI2C->RxSize = len; //Rxsize is used in the ISR code to manage the data reception
		hI2C->DevAddr = addr;
		hI2C->repeat = repeatedStart;

		//Implement code to Generate START Condition
		I2C_generateStartCondition(hI2C->instance);

		//Implement the code to enable ITBUFEN Control Bit
		hI2C->instance->CR2 |= I2C_CR2_ITBUFEN;

		//Implement the code to enable ITEVFEN Control Bit
		hI2C->instance->CR2 |= I2C_CR2_ITEVTEN;


		//Implement the code to enable ITERREN Control Bit
		hI2C->instance->CR2 |= I2C_CR2_ITERREN;

	}

	return busystate;
}

void I2C_SlaveTx(void);

void I2C_SlaveRx(void);

void I2C_ErrIRQHandler(I2C_Handle_t *hI2C)
{
	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (hI2C->instance->CR2) & (I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (hI2C->instance->SR1) & (I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		hI2C->instance->SR1 &= ~(I2C_SR1_BERR);

		//Implement the code to notify the application about the error

		I2C_ApplicationErrorCallback(hI2C,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (hI2C->instance->SR1) & (I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		hI2C->instance->SR1 &= ~(I2C_SR1_ARLO);

		//Implement the code to notify the application about the error

		I2C_ApplicationErrorCallback(hI2C,I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (hI2C->instance->SR1) & (I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		hI2C->instance->SR1 &= ~(I2C_SR1_AF);


		//Implement the code to notify the application about the error
		I2C_ApplicationErrorCallback(hI2C,I2C_ERROR_AF);

	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (hI2C->instance->SR1) & (I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		hI2C->instance->SR1 &= ~(I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationErrorCallback(hI2C,I2C_ERROR_OVR);

	}

/***********************Check for Time out error************************************/
	temp1 = (hI2C->instance->SR1) & (I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		hI2C->instance->SR1 &= ~(I2C_SR1_TIMEOUT);


		//Implement the code to notify the application about the error
		I2C_ApplicationErrorCallback(hI2C,I2C_ERROR_TIMEOUT);

	}
}

void I2C_EventIRQHandler(I2C_Handle_t *hI2C)
{

	uint32_t temp1, temp2, status_flags;

	temp1 = hI2C->instance->CR2 & I2C_CR2_ITEVTEN;
	temp2 = hI2C->instance->CR2 & I2C_CR2_ITBUFEN;
	status_flags = hI2C->instance->SR1 & I2C_SR1_SB;

	I2C_State_t state = hI2C->State;

	if(temp1 && status_flags)
	{
		if(state == I2C_BUSY_IN_RX)
			I2C_executeAddressPhase(hI2C->instance, hI2C->DevAddr, READ);
		else if(state == I2C_BUSY_IN_TX)
			I2C_executeAddressPhase(hI2C->instance, hI2C->DevAddr, WRITE);
	}

	status_flags = hI2C->instance->SR1;

	if(temp1 && (status_flags & I2C_SR1_ADDR))
	{
		I2C_clearADDRFlag(hI2C);
	}

	if(temp1 && (status_flags & I2C_SR1_BTF))
	{
		if(state == I2C_BUSY_IN_TX)
		{
			if(hI2C->TxLen == 0)
			{
				if(hI2C->repeat == I2C_NO_REPEAT)
					I2C_generateStopCondition(hI2C->instance);

				I2C_CloseCommunication(hI2C);

				I2C_ApplicationEventCallback(hI2C, I2C_EV_TX_CMPLT);
			}
		}else if(hI2C->State == I2C_BUSY_IN_RX && hI2C->RxLen <= 3)
		{
			if(hI2C->RxLen == 3)
			{
				__I2C_DISABLEACK(hI2C->instance);
				*hI2C->pRxBuffer++ = hI2C->instance->DR;
				hI2C->RxLen--;
			}else if(hI2C->RxLen==2)
			{
				if(hI2C->repeat == I2C_NO_REPEAT)
					I2C_generateStopCondition(hI2C->instance);
				if(hI2C->RxSize==2)
					__I2C_DISABLEPOS(hI2C->instance);

				if(hI2C->config.Ack == I2C_ACK_ENABLE)
					__I2C_ENABLEACK(hI2C->instance);
				*hI2C->pRxBuffer = hI2C->instance->DR;
				hI2C->pRxBuffer++;
				hI2C->RxLen--;

				*hI2C->pRxBuffer = hI2C->instance->DR;
				hI2C->pRxBuffer++;
				hI2C->RxLen--;



				I2C_CloseCommunication(hI2C);
				I2C_ApplicationEventCallback(hI2C, I2C_EV_RX_CMPLT);
			}

		}
	}

	if(temp1 && (status_flags & I2C_SR1_STOPF))
	{
		  hI2C->instance->CR1 |= I2C_CR1_PE; // CLEAR STOPF WITH DUMMY WRITE
		  I2C_ApplicationEventCallback(hI2C, I2C_EV_STOP);
	}


	if(temp1 && temp2 && (status_flags & I2C_SR1_TXE))
	{
		if(hI2C->instance->SR2 & I2C_SR2_MSL)
		{
			if(state == I2C_BUSY_IN_TX)
			{
				if(hI2C->TxLen > 0)
				{
					hI2C->instance->DR = *(hI2C->pTxBuffer++);

					hI2C->TxLen--;

				}
			}
		}else if(hI2C->instance->SR2 & I2C_SR2_TRA)
		{
			I2C_ApplicationEventCallback(hI2C,I2C_EV_DATA_REQ);
		}
	}


	if(temp1 && temp2 && (status_flags & I2C_SR1_RXNE))
	{
		if(hI2C->instance->SR2 & I2C_SR2_MSL)
		{
			if(state == I2C_BUSY_IN_RX)
			{
				if(hI2C->RxLen>3)
				{
					*hI2C->pRxBuffer++ = hI2C->instance->DR;
					hI2C->RxLen--;
				}else if(hI2C->RxLen == 1)
				{
					*hI2C->pRxBuffer++ = hI2C->instance->DR;
					hI2C->RxLen--;

					if(hI2C->config.Ack == I2C_ACK_ENABLE)
						__I2C_ENABLEACK(hI2C->instance);

					I2C_CloseCommunication(hI2C);
					I2C_ApplicationEventCallback(hI2C, I2C_EV_RX_CMPLT);
				}
			}
		}else if(hI2C->instance->SR2 & I2C_SR2_TRA)
		{
			I2C_ApplicationEventCallback(hI2C,I2C_EV_DATA_RCV);
		}

	}

}

void I2C_IRQInterruptConfig(IRQn_t IRQn, uint8_t ENorDI)
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

void I2C_IRQPriorityConfig(IRQn_t IRQn, uint8_t priority)
{
	NVIC->IPR[((uint32_t)IRQn) >> 2U] &= ~(0xFFU << (((uint32_t)(IRQn) & 0x3U)*8));
	NVIC->IPR[((uint32_t)IRQn) >> 2U] |= (uint32_t)(((priority&0x0F)<<__NVIC_PRIO_BITS)<<(((uint32_t)(IRQn) & 0x3U)*8));
}


void I2C_PeripheralControl(I2C_RegDef_t *instance, uint8_t ENorDI)
{
	if(ENorDI==ENABLE)
		instance->CR1 |= I2C_CR1_PE;
	else
		instance->CR1 &= I2C_CR1_PE;
}
uint8_t I2C_getFlag(I2C_RegDef_t *instance, uint32_t flag)
{
	 if(instance->SR1 & flag)
		 return SET;
	 else
		 return RESET;
}



void I2C_CloseCommunication(I2C_Handle_t *hI2C)
{

		hI2C->instance->CR2 &= ~I2C_CR2_ITBUFEN;

		hI2C->instance->CR2 &= ~I2C_CR2_ITEVTEN;

		if(hI2C->State == I2C_BUSY_IN_RX)
		{
			hI2C->pRxBuffer = NULL;
			hI2C->RxLen = 0;
			hI2C->RxSize = 0;
			if(hI2C->config.Ack == I2C_ACK_ENABLE)
			{
				__I2C_ENABLEACK(hI2C->instance);
			}
		}else if(hI2C->State == I2C_BUSY_IN_TX)
		{
			hI2C->pTxBuffer = NULL;
			hI2C->TxLen = 0;
		}
		hI2C->State = I2C_READY;

}
