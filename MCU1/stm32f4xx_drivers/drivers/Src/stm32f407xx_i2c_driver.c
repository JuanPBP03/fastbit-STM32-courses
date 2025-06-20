/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Jun 10, 2025
 *      Author: juanp
 */


#include "stm32f407xx_i2c_driver.h"


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
static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_executeAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t addr);
static void I2C_clearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx);

static void I2C_generateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= I2C_CR1_STOP;

}

static void I2C_clearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyread = pI2Cx->SR1;
	dummyread = pI2Cx->SR2;
	(void)dummyread;
}

static void I2C_executeAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t addr)
{
	addr = addr<<1;
	addr &= ~(1);	// Peripheral addr is addr + r/nw bit

	pI2Cx->DR = addr;
}

static void I2C_generateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= I2C_CR1_START;
}


void I2C_periClockCtrl(I2C_Channel_t ch, uint8_t ENorDI)
{

	if((ch>2)||(ch<0)) return;
	if(ENorDI==ENABLE)
		RCC->APB1ENR |= (1UL<<(21+ch));
	else if(ENorDI==DISABLE)
		RCC->APB1ENR &= ~(1UL<<(21+ch));
}

void I2C_Init(I2C_Handle_t *p_I2CHandle)
{
	I2C_periClockCtrl(p_I2CHandle->channel, ENABLE);



	if (p_I2CHandle->config.Ack == I2C_ACK_ENABLE)
		p_I2CHandle->instance->CR1 |= I2C_CR1_ACK;
	else
		p_I2CHandle->instance->CR1 &= I2C_CR1_ACK;


	uint16_t ccr;
	uint32_t PCLK1 = RCC_GetPCLK1Freq();
	if(p_I2CHandle->config.ClockSpeed <= I2C_SPEED_STANDARD)
	{
		p_I2CHandle->instance->CCR &= ~I2C_CCR_FS;
		ccr = PCLK1 / (2*p_I2CHandle->config.ClockSpeed);
		ccr = (ccr < 4) ? 4 : ccr;
		p_I2CHandle->instance->TRISE &= ~0x3FUL;
		p_I2CHandle->instance->TRISE |= (0x3FUL)&((RCC_GetPCLK1Freq()/1000000)+1);

	}
	else {

		p_I2CHandle->instance->CCR |= I2C_CCR_FS;
		if(p_I2CHandle->config.DutyCycle == I2C_FM_DUTY_2){
			p_I2CHandle->instance->CCR &= ~I2C_CCR_DUTY;
			ccr = PCLK1 / (3*p_I2CHandle->config.ClockSpeed);
		}else{
			p_I2CHandle->instance->CCR |= I2C_CCR_DUTY;
			ccr = PCLK1 / (25*p_I2CHandle->config.ClockSpeed);
		}
		p_I2CHandle->instance->TRISE &= ~0x3FUL;
		p_I2CHandle->instance->TRISE |= (0x3FUL)&( ( (RCC_GetPCLK1Freq()*300) / 1000000000)+1);
	}
	p_I2CHandle->instance->CCR &= ~I2C_CCR_CCR;
	p_I2CHandle->instance->CCR |= (ccr & I2C_CCR_CCR_Msk);
	p_I2CHandle->instance->CR2 &= ~I2C_CR2_FREQ;
	p_I2CHandle->instance->CR2 |= (PCLK1 & I2C_CR2_FREQ);
	p_I2CHandle->instance->OAR1 |= p_I2CHandle->config.OwnAddr<<1;
	p_I2CHandle->instance->OAR1 |= (1UL<<14);


}
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterTx(I2C_Handle_t *hI2C, uint8_t *pTxBuffer, uint32_t len, uint8_t addr)
{
	I2C_generateStartCondition(hI2C->instance);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_SB));

	I2C_executeAddressPhase(hI2C->instance, addr);
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_ADDR));
	I2C_clearADDRFlag(hI2C->instance);
	while(len>0){
		while(!I2C_getFlag(hI2C->instance, I2C_FLAG_TXE));
		hI2C->instance->DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_TXE));
	while(!I2C_getFlag(hI2C->instance, I2C_FLAG_BTF));

	I2C_generateStopCondition(hI2C->instance);

}



void I2C_MasterRx(void);

void I2C_SlaveTx(void);

void I2C_SlaveRx(void);

void I2C_ErrIRQHandler(void);

void I2c_EventIRQHandler(void);

void I2C_IRQInterruptConfig(IRQn_t IRQNumber, uint8_t ENorDI);
void I2C_IRQInterruptConfig(IRQn_t IRQn, uint8_t priority);


void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t ENorDI)
{
	if(ENorDI==ENABLE)
		pI2Cx->CR1 |= I2C_CR1_PE;
	else
		pI2Cx->CR1 &= I2C_CR1_PE;
}
uint8_t I2C_getFlag(I2C_RegDef_t *pI2Cx, uint32_t flag)
{
	 if(pI2Cx->SR1 & flag)
		 return SET;
	 else
		 return RESET;
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);
