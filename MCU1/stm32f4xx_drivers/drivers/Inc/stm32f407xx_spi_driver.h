/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Jun 7, 2025
 *      Author: JuanP
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"
#include <stdbool.h>

/*
 *  SPI config
 */

typedef enum{
	SPI_CH1 = 0,
	SPI_CH2,
	SPI_CH3,
	SPI_CHANNELS
}SPI_Channel_t;

typedef enum{
	SPI_FLAG_RXRDY = 0,
	SPI_FLAG_TXRDY,
	SPI_FLAG_CHSIDE,
	SPI_FLAG_UNDERRUN,
	SPI_FLAG_CRCERR,
	SPI_FLAG_MODEFAULT,
	SPI_FLAG_OVERRUN,
	SPI_FLAG_BUSY,
	SPI_FLAG_FORMATERR
}SPI_StatusFlags_t;



typedef enum{
	SPI_MODE_SLAVE=0,
	SPI_MODE_MASTER
}SPI_Mode_t;

typedef enum{
	SPI_CLK_2 = 0,
	SPI_CLK_4,
	SPI_CLK_8,
	SPI_CLK_16,
	SPI_CLK_32,
	SPI_CLK_64,
	SPI_CLK_128,
	SPI_CLK_256
}SPI_baudPrescaler_t;

typedef enum{
	SPI_FRAME_SIZE_8_BIT = 0,
	SPI_FRAME_SIZE_16_BIT
}SPI_FrameSize_t;

typedef enum{
	SPI_COMM_MODE_FULL = 0,
	SPI_COMM_MODE_HALF_BI,
	SPI_COMM_MODE_HALF_UNI,
	SPI_COMM_MODE_SIMPLEX_RXONLY,

}SPI_CommMode_t;

typedef enum{
	SPI_SSM_HARDWARE = 0,
	SPI_SSM_SOFTWARE
}SPI_SlaveSelect_t;

typedef enum{
	SPI_PROTOCOL_MOTOROLA = 0,
	SPI_PROTOCOL_TI
}SPI_Protocol_t;

typedef enum{
	SPI_DMA_NONE = 0,
	SPI_DMA_RX,
	SPI_DMA_TX,
	SPI_DMA_RXTX
}SPI_DMA_t;

typedef enum{
	SPI_EDGE_FIRST = 0,
	SPI_EDGE_SECOND
}SPI_ClkEdge_t;

typedef enum{
	SPI_IDLE_LOW = 0,
	SPI_IDLE_HIGH
}SPI_ClkIdle_t;

typedef enum{
	SPI_FORMAT_MSB = 0,
	SPI_FORMAT_LSB
}SPI_bitOrder_t;

typedef struct{
	SPI_Mode_t Mode;					//	Master/slave
	SPI_baudPrescaler_t baudPrescaler; 	//	Baud rate prescaler
	SPI_FrameSize_t frameSize;		//	Frame Format
	SPI_CommMode_t commMode;			//	Communication mode (Full duplex/Half bidirectional/Half unidirectional/Simplex)
	SPI_SlaveSelect_t slaveSelectMode;				//	NSS management (software or hardware)

	//	OPTIONAL:
	SPI_Protocol_t frameProtocol;			//	Frame Protocol (TI or Motorola)
	bool CRCEnable;						//	CRC mode
	uint16_t CRCPoly;					//	CRC poly
	SPI_DMA_t DMAEnable;				//	DMA mode
	SPI_ClkEdge_t clkPol;			//	Clock Phase
	SPI_ClkIdle_t clkPhase;					//	Clock Polarity
	SPI_bitOrder_t bitOrder;			//	Bit order
	bool TxIRQ;
	bool RxIRQ;
	bool ErrIRQ;
}SPI_Config_t;

extern SPI_I2S_RegDef_t *const SPI[SPI_CHANNELS];
extern volatile uint32_t *const SPI_CLK_EN_REGS[SPI_CHANNELS];
extern const uint32_t SPI_RCC_MASKS[SPI_CHANNELS];
extern volatile uint32_t *const SPI_RCC_RST_REGS[SPI_CHANNELS];

typedef struct{
	SPI_Channel_t channel;
	SPI_Config_t SPIConfig;
}SPI_Handle_t;


void SPI_periClockEnable(SPI_Channel_t channel);

bool SPI_getFlag(SPI_Channel_t channel, SPI_StatusFlags_t flag);

void SPI_Reset(SPI_Channel_t channel);

void SPI_Init(SPI_Handle_t *p_SPIHandle);

void SPI_Enable(SPI_Channel_t channel);

void SPI_Disable(SPI_Channel_t channel);

void SPI_Tx16(SPI_Channel_t channel, uint16_t *buffer, uint32_t len);

void SPI_Rx16(SPI_Channel_t channel, uint16_t *buffer, uint32_t len);

void SPI_Tx8(SPI_Channel_t channel, uint8_t *buffer, uint32_t len);

void SPI_Rx8(SPI_Channel_t channel, uint8_t *buffer, uint32_t len);

void SPI_Transmit_IT(SPI_Channel_t ch, uint8_t *buffer, uint32_t len);

void SPI_IRQConfig(SPI_Channel_t channel, bool ENorDI );
void SPI_IRQPriority(SPI_Channel_t channel, uint8_t priority);

void SPI_RxIRQEn(SPI_Channel_t channel);
void SPI_RxIRQDis(SPI_Channel_t channel);

void SPI_TxIRQEn(SPI_Channel_t channel);
void SPI_TxIRQDis(SPI_Channel_t channel);

void SPI_ErrIRQEn(SPI_Channel_t channel);
void SPI_ErrIRQDis(SPI_Channel_t channel);

void SPI_IRQHandler(SPI_Channel_t channel);


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
