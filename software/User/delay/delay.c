#include "delay.h"


//如果使用OS,则包括下面的头文件即可
#if      1   							//SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h"
#endif

static uint8_t  fac_us=0;							//us延时倍乘数			   
static uint16_t fac_ms=0;							//ms延时倍乘数,在os下,代表每个节拍的ms数

 
extern void xPortSysTickHandler(void);
/**********************************************************************************************************
*函 数 名: void SysTick_Handler(void)
*功能说明: systick中断服务函数,使用OS时用到
*形    参: 
*返 回 值: 
**********************************************************************************************************/	 
//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}

/**********************************************************************************************************
*函 数 名: void delay_init(uint8_t SYSCLK)
*功能说明: 初始化延迟函数 SYSTICK的时钟固定为AHB时钟，基础例程里面SYSTICK时钟频率为AHB/8
*形    参: SYSCLK:系统时钟频率 这里为了兼容FreeRTOS，所以将SYSTICK的时钟频率改为AHB的频率！
*返 回 值: 无
**********************************************************************************************************/			   
void delay_init(uint8_t SYSCLK)
{
	uint32_t reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;							//每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;		//根据delay_ostickspersec设定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
}	

/**********************************************************************************************************
*函 数 名: void delay_us(uint32_t nus)
*功能说明: 延时us
*形    参: nus:要延时的us数.nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	
*返 回 值: 无
**********************************************************************************************************/	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
}

/**********************************************************************************************************
*函 数 名: void delay_ms(uint32_t nms)
*功能说明: 延时ms
*形    参: nms:要延时的ms数 nms:0~65535
*返 回 值: 无
**********************************************************************************************************/	 
void delay_ms(uint32_t nms)
{	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if(nms>=fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms/fac_ms);	 		//FreeRTOS延时
		}
		nms%=fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((uint32_t)(nms*1000));				//普通方式延时
}

/**********************************************************************************************************
*函 数 名: void delay_xms(uint32_t nms)
*功能说明: 延时nms,不会引起任务调度
*形    参: nms:要延时的ms数
*返 回 值: 无
**********************************************************************************************************/
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/**********************************************************************************************************
*函 数 名: GetSysTimeUs
*功能说明: 获取当前系统运行时间，单位为微秒
*形    参: 无
*返 回 值: 系统时间
**********************************************************************************************************/
uint64_t GetSysTimeUs(void)
{
    uint64_t ms;
    uint64_t value;
    ms = xTaskGetTickCount();
    value = ms * 1000 + (SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD;
    return value;
}

/**********************************************************************************************************
*函 数 名: GetSysTimeMs
*功能说明: 获取当前系统运行时间，单位为毫秒
*形    参: 无
*返 回 值: 系统时间
**********************************************************************************************************/
uint32_t GetSysTimeMs(void)
{
    return xTaskGetTickCount();
}

			 



































