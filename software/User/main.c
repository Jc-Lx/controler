#include "TaskConfig.h"

xTaskHandle startTask;


static void BSP_Init(void);

//ȫ�ֱ��������ڱ���74HC165ɨ�践�صİ���ֵ
extern uint8_t HC_Recieve_Date ;

//ADCת���ĵ�ѹֵͨ��DMA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[ ADC_DATA ];

/**********************************************************************************************************
*�� �� ��: portTASK_FUNCTION(vStartTask, pvParameters)
*����˵��: ϵͳ�������񣬵��ø����ʼ����������������Ϣ���к�Ҫ���е��û�����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
portTASK_FUNCTION(vStartTask, pvParameters)
{
  //Ӳ����ʼ��
  BSP_Init();

  /*****************************�û����񴴽�********************************/  
  taskENTER_CRITICAL();
  
  //����������ҡ���źŶ�ȡ����
  ReadTaskCreate();

  //���������źŷ�������
  SendTaskCreate();
  
  vTaskDelete(startTask); 
  
  taskEXIT_CRITICAL();            //�˳��ٽ���


  for(;;)
  {
    vTaskDelay(5000);
  }
}

/**********************************************************************************************************
*�� �� ��: int main(void)
*����˵��: ϵͳ�������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;
  //������������
  xReturn = xTaskCreate(vStartTask,"startTask",128,NULL,0,&startTask);
  /* ����������� */           
  if(pdPASS == xReturn)
  {
    vTaskStartScheduler();   /* �������񣬿������� */
    printf("\r\n    ϵͳ��ʼ�����    \r\n");
  }
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}

/**********************************************************************************************************
*�� �� ��: static void BSP_Init(void)
*����˵��: Ӳ����ʼ������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void BSP_Init(void)
{

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();

	/* ���ڳ�ʼ��	*/
	Debug_USART_Config();

  /* 74hc165��ʼ��*/
  HC165_GPIO_Config();

  /* ADC��ʼ��*/
  JS_ADC_Init();

  //��ʼ����ʱ����
  delay_init(168);

  //NRFL2401P��ʼ��
  NRF24L01P_Init();	

  //BEEPER��ʼ��
  BEEPER_Init();				
  
}

/********************************END OF FILE****************************/
