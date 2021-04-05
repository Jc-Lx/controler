#include "TaskConfig.h"

xTaskHandle startTask;


static void BSP_Init(void);

//全局变量，用于保存74HC165扫描返回的按键值
extern uint8_t HC_Recieve_Date ;

//ADC转换的电压值通过DMA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[ ADC_DATA ];

/**********************************************************************************************************
*函 数 名: portTASK_FUNCTION(vStartTask, pvParameters)
*功能说明: 系统启动任务，调用各类初始化函数，并创建消息队列和要运行的用户任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
portTASK_FUNCTION(vStartTask, pvParameters)
{
  //硬件初始化
  BSP_Init();

  /*****************************用户任务创建********************************/  
  taskENTER_CRITICAL();
  
  //创建按键与摇杆信号读取任务
  ReadTaskCreate();

  //创建控制信号发送任务
  SendTaskCreate();
  
  vTaskDelete(startTask); 
  
  taskEXIT_CRITICAL();            //退出临界区


  for(;;)
  {
    vTaskDelay(5000);
  }
}

/**********************************************************************************************************
*函 数 名: int main(void)
*功能说明: 系统程序入口
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;
  //创建启动任务
  xReturn = xTaskCreate(vStartTask,"startTask",128,NULL,0,&startTask);
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
  {
    vTaskStartScheduler();   /* 启动任务，开启调度 */
    printf("\r\n    系统初始化完成    \r\n");
  }
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}

/**********************************************************************************************************
*函 数 名: static void BSP_Init(void)
*功能说明: 硬件初始化函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void BSP_Init(void)
{

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* 串口初始化	*/
	Debug_USART_Config();

  /* 74hc165初始化*/
  HC165_GPIO_Config();

  /* ADC初始化*/
  JS_ADC_Init();

  //初始化延时函数
  delay_init(168);

  //NRFL2401P初始化
  NRF24L01P_Init();	

  //BEEPER初始化
  BEEPER_Init();				
  
}

/********************************END OF FILE****************************/
