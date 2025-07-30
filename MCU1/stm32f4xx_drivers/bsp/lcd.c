/*
 * lcd.c
 *
 *  Created on: Jul 23, 2025
 *      Author: JuanP
 */
#include "lcd.h"

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

void lcd_send_command(uint8_t cmd)
{
	/* RS=0 for LCD command */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, SET);

	/*R/nW = 0, for write */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, RESET);

	write_4_bits(cmd >> 4);
	write_4_bits(cmd & 0x0F);

}

void lcd_print_char(uint8_t data)
{
	/* RS=1 for LCD user data */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, SET);

	/*R/nW = 0, for write */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, SET);

	write_4_bits(data >> 4);  /*Higher nibble*/
	write_4_bits(data & 0x0F); /*Lower nibble*/


}

void lcd_print_string(uint8_t *message)
{

      do
      {
          lcd_print_char((uint8_t)*message++);
      }
      while (*message != '\0');

}

void lcd_init(void)
{

	//1. Configure the gpio pins which are used for lcd connections

	GPIO_Handle_t lcd_signal;

	lcd_signal.p_GPIOx = LCD_GPIO_PORT;
	lcd_signal.GPIO_PinConfig.pinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_RS;
	lcd_signal.GPIO_PinConfig.pinOType = GPIO_OTYPE_PUSHPULL;
	lcd_signal.GPIO_PinConfig.pinPUPDCtrl = GPIO_PUPD_NONE;
	lcd_signal.GPIO_PinConfig.pinSpeed = GPIO_SPEED_HIGH;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_RW;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_EN;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_D4;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_D5;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_D6;
	GPIO_PinInit(&lcd_signal);

	lcd_signal.GPIO_PinConfig.pinNumber = LCD_GPIO_D7;
	GPIO_PinInit(&lcd_signal);

	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, RESET);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, RESET);

	//2. Do the LCD initialization

	mdelay(40);

	/*RS = 0 , For LCD command */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, RESET);

	/* RnW = 0, Writing to LCD */
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, RESET);

	write_4_bits(0x3);

	udelay(37);

	write_4_bits(0x2);
	write_4_bits(0x8);


	udelay(37);

	//function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//disply ON and cursor ON
	lcd_send_command(0x0F);
	while(lcd_isBusy());

	lcd_display_clear();
	while(lcd_isBusy());

	//entry mode set
	lcd_send_command(LCD_CMD_INCADD);
	while(lcd_isBusy());

}

static void write_4_bits(uint8_t value){

	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, ((value>>0) & 1));
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, ((value>>1) & 1));
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, ((value>>2) & 1));
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, ((value>>3) & 1));

	lcd_enable();

}


static void lcd_enable(void)
{
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, SET);
	udelay(10);
	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, RESET);
}

uint8_t lcd_isBusy(void){

	GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, ENABLE);
	return GPIO_ReadPin(LCD_GPIO_PORT, LCD_GPIO_D7);

}

void lcd_display_clear(void){
	lcd_send_command(LCD_CMD_DIS_CLEAR);
}

void lcd_display_return_home(void)
{

	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);

}

void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;
  switch (row)
  {
    case 1:
      /* Set cursor to 1st row address and add index*/
      lcd_send_command((column |= 0x80));
      break;
    case 2:
      /* Set cursor to 2nd row address and add index*/
       lcd_send_command((column |= 0xC0));
      break;
    default:
      break;
  }
}

static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 16000); i++);
}

static void udelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 16); i++);
}
