/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Jun 7, 2025
 *      Author: JuanP
 */

#include "stm32f407xx_spi_driver.h"
#include <stdbool.h>


SPI_I2S_RegDef_t *const SPI[SPI_CHANNELS] = {
		SPI1,
		SPI2,
		SPI3
};
volatile uint32_t *const SPI_RCC_EN_REGS[SPI_CHANNELS] = {
		&RCC->APB2ENR,
		&RCC->APB1ENR,
		&RCC->APB1ENR
};
volatile uint32_t *const SPI_RCC_RST_REGS[SPI_CHANNELS] = {
		&RCC->APB2RSTR,
		&RCC->APB1RSTR,
		&RCC->APB1RSTR
};
const uint32_t SPI_RCC_MASKS[SPI_CHANNELS] = {
		(1<<12),
		(1<<14),
		(1<<15)
};

static const IRQn_t SPI_IRQn[SPI_CHANNELS] = {
		IRQn_SPI1,
		IRQn_SPI2,
		IRQn_SPI3
};

typedef struct {
    uint8_t *pTxBuffer;
    uint32_t txLen;
    bool busy;
} SPI_TxContext_t;

static SPI_TxContext_t spiTxCtx[SPI_CHANNELS];  // One context per SPI


void SPI_periClockEnable(SPI_Channel_t channel)
{
	*SPI_RCC_EN_REGS[channel] |= SPI_RCC_MASKS[channel];
}

bool SPI_getFlag(SPI_Channel_t channel, SPI_StatusFlags_t flag)
{
	return (((SPI[channel]->SR)>>flag) & 1U);
}


void SPI_Reset(SPI_Channel_t channel)
{
	*SPI_RCC_RST_REGS[channel] |= SPI_RCC_MASKS[channel];
	*SPI_RCC_RST_REGS[channel] &= ~SPI_RCC_MASKS[channel];

}

void SPI_Init(SPI_Handle_t *p_SPIHandle)
{
	uint8_t channel = (uint8_t)(p_SPIHandle->channel);
	SPI_Config_t config = p_SPIHandle->SPIConfig;
	// 0. Reset to default state
	SPI_Reset(channel);

	//	Communication mode setup
	switch(config.commMode){

	case SPI_COMM_MODE_FULL:
		//	Default state: BIDIMODE = 0, RXONLY = 0
		break;
	case SPI_COMM_MODE_HALF_BI:
		SPI[channel]->CR1 |= (1U<<SPI_CR1_BIDIMODE_Pos);
		//	BIDIOE will be toggled during runtime depending on TX/RX
		break;
	case SPI_COMM_MODE_HALF_UNI:
		//	BIDIMODE = 0, RXONLY = 0,
		//	Ignore incoming data in Tx mode
		//	Set RXONLY bit in Rx mode
		break;
	case SPI_COMM_MODE_SIMPLEX_RXONLY:
		SPI[channel]->CR1 |= (1U<<SPI_CR1_RXONLY_Pos);	//	RXONLY
		break;

	}
	if(config.Mode == SPI_MODE_MASTER)
	{
		SPI[channel]->CR1 |= ((config.baudPrescaler)<<SPI_CR1_BR_Pos);			//	Baud Rate
		SPI[channel]->CR1 |= ((config.clkPol)<<SPI_CR1_CPOL_Pos);				//	Clock Polarity
		SPI[channel]->CR1 |= ((config.clkPhase)<<SPI_CR1_CPHA_Pos);			//	Clock Phase
		SPI[channel]->CR1 |= ((config.frameSize)<<SPI_CR1_DFF_Pos);			//	Data Frame Format
		SPI[channel]->CR1 |= ((config.bitOrder)<<SPI_CR1_LSBFIRST_Pos);		//	MSB or LSB
		if(!(config.frameProtocol)){
			SPI[channel]->CR1 |= ((config.slaveSelectMode)<<SPI_CR1_SSM_Pos);	//	SSM mode
			SPI[channel]->CR1 |= (1U<<SPI_CR1_SSI_Pos);							// Set SSI bit (required when SSM = 1 in master mode)
			SPI[channel]->CR2 &= ~(1U<<SPI_CR2_FRF_Pos);						//  Motorola

		}else SPI[channel]->CR2 |= (1<<SPI_CR2_FRF_Pos);						//	TI

		SPI[channel]->CR1 |= ((config.Mode)<<SPI_CR1_MSTR_Pos);				//	Master mode
	}
	else if (config.Mode == SPI_MODE_SLAVE)
	{
		SPI[channel]->CR1 |= ((config.frameSize)<<SPI_CR1_DFF_Pos);			//	Data Frame Format
		SPI[channel]->CR1 |= ((config.clkPol)<<SPI_CR1_CPOL_Pos);				//	Clock Polarity
		SPI[channel]->CR1 |= ((config.clkPhase)<<SPI_CR1_CPHA_Pos);			//	Clock Phase
		SPI[channel]->CR1 |= ((config.bitOrder)<<SPI_CR1_LSBFIRST_Pos);		//	MSB or LSB
		if(!(config.frameProtocol)){
			SPI[channel]->CR1 |= ((config.slaveSelectMode)<<SPI_CR1_SSM_Pos);	//	SSM mode
			SPI[channel]->CR1 &= ~(1U<<SPI_CR1_SSI_Pos);						//	Clear SSI bit (Data sheet procedure)
			SPI[channel]->CR2 &= ~(1U<<SPI_CR2_FRF_Pos);						//  Motorola

		}else SPI[channel]->CR2 |= (1<<SPI_CR2_FRF_Pos);						//	TI

		SPI[channel]->CR1 &= ~(1U<<SPI_CR1_MSTR_Pos);							//	Slave mode
	}

}

void SPI_Enable(SPI_Channel_t channel) {
    SPI[channel]->CR1 |= (1U << 6);  // SPE
}

void SPI_Disable(SPI_Channel_t channel) {
    SPI[channel]->CR1 &= ~(1U << 6);  // SPE
}

void SPI_Tx16(SPI_Channel_t channel, uint16_t *buffer, uint32_t len)
{
	uint16_t *p = buffer;

	while(len!=0){
		while (!(SPI_getFlag(channel, SPI_FLAG_TXRDY))); // TXE

		SPI[channel]->DR = *p;
		len--;
		p++;
	}
}

void SPI_Rx16(SPI_Channel_t channel, uint16_t *buffer, uint32_t len)
{
	while(len!=0){
		while (!(SPI_getFlag(channel, SPI_FLAG_RXRDY))); // RXNE
		*buffer = SPI[channel]->DR;
		len--;
		buffer++;
	}
}

void SPI_Tx8(SPI_Channel_t channel, uint8_t *buffer, uint32_t len)
{
	uint8_t *p = buffer;
	while(len!=0){
		while (!(SPI_getFlag(channel, SPI_FLAG_TXRDY))); // TXE

		SPI[channel]->DR = *p;
		len --;
		p++;
	}
}

void SPI_Rx8(SPI_Channel_t channel, uint8_t *buffer, uint32_t len)
{

	while(len!=0){
		while (!(SPI_getFlag(channel, SPI_FLAG_RXRDY))); // RXNE
		*buffer = SPI[channel]->DR;
		len--;
		buffer++;
	}
}

bool SPI_IsBusy(SPI_Channel_t channel)
{
	return (SPI[channel]->SR & (1U << 7));  // BSY
}

void SPI_IRQConfig(SPI_Channel_t channel, bool ENorDI)
{

	uint8_t IRQn = SPI_IRQn[channel];

	if(ENorDI == ENABLE){
		NVIC->ISER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
	}
	else
	{
		NVIC->ICER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
	}


}


void SPI_RxIRQEn(SPI_Channel_t channel)
{
	SPI[channel]->CR2 |= (1<<SPI_CR2_RXNEIE_Pos);
}
void SPI_RxIRQDis(SPI_Channel_t channel){
	SPI[channel]->CR2 &= ~(1<<SPI_CR2_RXNEIE_Pos);
}


void SPI_TxIRQEn(SPI_Channel_t channel)
{
	SPI[channel]->CR2 |= (1<<SPI_CR2_TXEIE_Pos);
}

void SPI_TxIRQDis(SPI_Channel_t channel)
{
	SPI[channel]->CR2 &= ~(1<<SPI_CR2_TXEIE_Pos);
}


void SPI_ErrIRQEn(SPI_Channel_t channel)
{
	SPI[channel]->CR2 |= (1<<SPI_CR2_ERRIE_Pos);
}
void SPI_ErrIRQDis(SPI_Channel_t channel)
{
	SPI[channel]->CR2 &= ~(1<<SPI_CR2_ERRIE_Pos);
}

void SPI_Transmit_IT(SPI_Channel_t ch, uint8_t *buffer, uint32_t len)
{
    if (spiTxCtx[ch].busy) return;  // Already transmitting

    spiTxCtx[ch].pTxBuffer = buffer;
    spiTxCtx[ch].txLen = len;
    spiTxCtx[ch].busy = true;


    SPI_TxIRQEn(ch);  // Enable TXE interrupt
    SPI_Enable(ch);

}

void SPI_IRQPriority(SPI_Channel_t channel, uint8_t priority)
{
	uint8_t IRQn = SPI_IRQn[channel];

	NVIC->IPR[((uint32_t)IRQn) >> 2U] &= ~(0xFFU << (((uint32_t)(IRQn) & 0x3U)*8));
	NVIC->IPR[((uint32_t)IRQn) >> 2U] |= (uint32_t)(((priority&0x0F)<<__NVIC_PRIO_BITS)<<(((uint32_t)(IRQn) & 0x3U)*8));
}

void SPI_IRQHandler(SPI_Channel_t channel)
{

	if((SPI_getFlag(channel, SPI_FLAG_TXRDY))&&(spiTxCtx[channel].txLen>0)){
		SPI[channel]->DR = *(spiTxCtx[channel].pTxBuffer++);
		spiTxCtx[channel].txLen--;

		if(spiTxCtx[channel].txLen==0){
			SPI_TxIRQDis(channel);
			spiTxCtx[channel].busy=false;

		}
	}
}

