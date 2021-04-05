#ifndef __BEEPER_H_
#define __BEEPER_H_

#include "stm32f4xx.h"
#include "bsp_led.h"

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 	 {p->ODR ^=i;}		//输出反转状态

/* 定义控制IO的宏 */
#define BEEPER_TOGGLE		digitalToggle(GPIOG,GPIO_Pin_11)
#define BEEPER_OFF			digitalHi(GPIOG,GPIO_Pin_11)
#define BEEPER_ON			digitalLo(GPIOG,GPIO_Pin_11)


void BEEPER_Init(void);
void ChangeTIM3time(uint32_t time);

#endif


