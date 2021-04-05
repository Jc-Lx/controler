#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"

//定义ADC读取数据存储数组大小的宏
#define       ADC_DATA                  4

/*==========================左手摇杆通道定义===========================*/

//JSL_X IO宏定义
#define       JSL_X_PIN                  GPIO_Pin_5
#define       JSL_X_GPIO_PORT            GPIOF
#define       JSL_X_GPIO_CLK             RCC_AHB1Periph_GPIOF
#define       JSL_X_ADC_CHANNEL          ADC_Channel_15

//JSL_Y IO宏定义
#define       JSL_Y_PIN                  GPIO_Pin_6
#define       JSL_Y_GPIO_PORT            GPIOF
#define       JSL_Y_GPIO_CLK             RCC_AHB1Periph_GPIOF
#define       JSL_Y_ADC_CHANNEL          ADC_Channel_4

// //JSL_LOCK IO宏定义
// #define       JSL_LOCK_PIN                  GPIO_Pin_7
// #define       JSL_LOCK_GPIO_PORT            GPIOF
// #define       JSL_LOCK_GPIO_CLK             RCC_AHB1Periph_GPIOF
// #define       JSL_LOCK_ADC_CHANNEL          ADC_Channel_5

/*==========================右手摇杆通道定义===========================*/

//JSR_X IO宏定义
#define       JSR_X_PIN                  GPIO_Pin_9
#define       JSR_X_GPIO_PORT            GPIOF
#define       JSR_X_GPIO_CLK             RCC_AHB1Periph_GPIOF
#define       JSR_X_ADC_CHANNEL          ADC_Channel_7

//JSR_Y IO宏定义
#define       JSR_Y_PIN                  GPIO_Pin_8
#define       JSR_Y_GPIO_PORT            GPIOF
#define       JSR_Y_GPIO_CLK             RCC_AHB1Periph_GPIOF
#define       JSR_Y_ADC_CHANNEL          ADC_Channel_6

// /*==========================云台摇杆通道定义===========================*/

// //YUNTAI IO宏定义
// #define       YUNTAI_PIN                  GPIO_Pin_10
// #define       YUNTAI_GPIO_PORT            GPIOF
// #define       YUNTAI_GPIO_CLK             RCC_AHB1Periph_GPIOF
// #define       YUNTAI_ADC_CHANNEL          ADC_Channel_8


//ADC 序号宏定义
#define       JS_ADC                      ADC3
#define       JS_ADC_CLK                  RCC_APB2Periph_ADC3
//ADC DR寄存器宏定义，ADC转换后的数值则存放在这里
#define       JS_ADC_DR_ADDR              ((u32)ADC3+0x4c)  



//ADC DMA通道宏定义，这里我们使用DMA传输
#define       JS_ADC_DMA_CLK              RCC_AHB1Periph_DMA2
#define       JS_ADC_DMA_CHANNEL          DMA_Channel_2
#define       JS_ADC_DMA_STREAM           DMA2_Stream0


void JS_ADC_Init(void);
#endif

