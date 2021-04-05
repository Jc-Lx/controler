#include "beeper.h"

/**********************************************************************************************************
*函 数 名: static void BEEPER_GPIO_Config(void)
*功能说明: BEEPER引脚初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void BEEPER_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

  	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化	
}

/**********************************************************************************************************
*函 数 名: static void BEEPER_TIM3_Config(void)
*功能说明: BEEPER定时器初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void BEEPER_TIM3_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = 5000; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=255-1;  //定时器分频 时基2us
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3

    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/**********************************************************************************************************
*函 数 名: void BEEPER_Init(void)
*功能说明: BEEP初始化函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void BEEPER_Init(void)
{
    BEEPER_GPIO_Config();
    BEEPER_TIM3_Config();
}

uint32_t TIM3time = 5000;
/**********************************************************************************************************
*函 数 名: void TIM3_IRQHandler(void)
*功能说明: 定时器3定时中断函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
        TIM3->ARR = TIM3time;
		BEEPER_TOGGLE;
	}
}
/**********************************************************************************************************
*函 数 名: void TIM3_IRQHandler(void)
*功能说明: 定时器3定时中断函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ChangeTIM3time(uint32_t time)
{
	TIM3time = time;
}

