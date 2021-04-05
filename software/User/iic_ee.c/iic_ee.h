#ifndef __IIC_EE_H
#define __IIC_EE_H

#include "stm32f4xx.h"
#include "soft_iic_gpio.h"
#include "delay.h"
#include "bsp_debug_usart.h"

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */
	

#define EEPROM_DEV_ADDR			0xA0		/* 24xx02的设备地址 */
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02的页面大小 */
#define EEPROM_SIZE				  8		  /* 24xx02总容量 */


uint8_t ee_CheckOk(void);
uint8_t ee_ReadOneByte(uint16_t ReadAddr);
uint8_t ee_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
uint8_t ee_ReadBytes(uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead);
uint8_t ee_WriteBytes(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
void ee_Erase(void);
//uint8_t ee_Test(void);

#endif





