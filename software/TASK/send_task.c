#include "TaskConfig.h"

TaskHandle_t sendtask;


//定义全局遥控器发送数组
extern  uint8_t   transport_date[2*ADC_DATA];

extern uint16_t lx_pwm,ly_pwm,rx_pwm,ry_pwm;
/**********************************************************************************************************
*函 数 名: vOtherSensorTask
*功能说明: 其它传感器数据预处理任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
portTASK_FUNCTION(vSendTask, pvParameters)
{
    uint8_t temp;
    while(NRF24L01P_Check() == 1)
    {
      LED1_TOGGLE;
      printf("\r\n    nrf dont work !    %d    \r\n",temp);
    }

    NRF24L01P_TX_Mode();

    while (1)
    {
      //摇杆信号编码
      PWN_Modulation();

      //解锁辅助判断
      ArmedAuxiliary();

      if(NRF24L01P_TxPacket(transport_date) == TX_OK)
      {
        printf("\r\n   pith  = %d us   roll  = %d us  yaw  = %d us   throttle  = %d us  \r\n",ry_pwm,rx_pwm,lx_pwm,ly_pwm);

        // for ( i = 0; i < 8; i++)
        // {
        //   printf("   %d   \n",transport_date[i]);
        // }        
      }
      else
      {
        printf("\r\n    send failed !    \r\n");
        LED3_TOGGLE;//绿灯
      }

      vTaskDelay(5); 
    };
}
/**********************************************************************************************************
*函 数 名: ModuleTaskCreate
*功能说明: 传感器组件相关任务创建
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SendTaskCreate(void)
{
    xTaskCreate(vSendTask,"sendtask",512,NULL,12,&sendtask);
}
