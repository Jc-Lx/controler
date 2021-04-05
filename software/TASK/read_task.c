#include "TaskConfig.h"

TaskHandle_t readtask;

/**********************************************************************************************************
*函 数 名: vOtherSensorTask
*功能说明: 其它传感器数据预处理任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
portTASK_FUNCTION(vReadTask, pvParameters)
{
    uint32_t tick;
    uint8_t temp; 
    for(;;)
    {
        tick ++;
        HC165_Scan();
        temp = BUTTON_Scan(KEY5);
        if(Getarmedflag() == 1)
        {
            LED1_ON;
        }else LED1_OFF;
        printf("%d\r\n",temp );

        if( tick % 10 == 0)  LED2_TOGGLE;//红灯

        vTaskDelay(10);   /* 延时500个tick 蓝色灯*/	 		 
    }
}

/**********************************************************************************************************
*函 数 名: ModuleTaskCreate
*功能说明: 传感器组件相关任务创建
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ReadTaskCreate(void)
{
    xTaskCreate(vReadTask,"readtask",512,NULL,13,&readtask);
}
