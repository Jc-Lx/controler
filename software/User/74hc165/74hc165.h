#ifndef __74HC165_H
#define __74HC165_H

#include "stm32f4xx.h"
#include "bsp_debug_usart.h"

//74hc165引脚定义
#define HC_PL_PIN           GPIO_Pin_2
#define HC_PL_GPIO_PORT     GPIOE
#define HC_PL_GPIO_CLK      RCC_AHB1Periph_GPIOE 

#define HC_CP_PIN           GPIO_Pin_3
#define HC_CP_GPIO_PORT     GPIOE
#define HC_CP_GPIO_CLK      RCC_AHB1Periph_GPIOE

#define HC_Q7_PIN           GPIO_Pin_4
#define HC_Q7_GPIO_PORT     GPIOE
#define HC_Q7_GPIO_CLK      RCC_AHB1Periph_GPIOE

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平

#define HC_PL_0              digitalLo(HC_PL_GPIO_PORT,HC_PL_PIN)
#define HC_PL_1              digitalHi(HC_PL_GPIO_PORT,HC_PL_PIN)

#define HC_CP_0              digitalLo(HC_CP_GPIO_PORT,HC_CP_PIN)
#define HC_CP_1              digitalHi(HC_CP_GPIO_PORT,HC_CP_PIN)


//定义按键状态宏
#define    DOWN      1
#define    UP        0

//定义扫描按键数值对应的按键宏
#define  KEY1    1
#define  KEY2    2
#define  KEY3    4
#define  KEY4    8
#define  KEY5    128
#define  KEY6    64
#define  KEY7    32
#define  KEY8    16

void HC165_GPIO_Config(void);
uint8_t HC165_Scan(void);

uint8_t BUTTON_Scan( uint8_t key);


#endif

