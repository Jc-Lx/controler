#include "adc.h"

__IO uint16_t ADC_ConvertedValue[ ADC_DATA ]={0};

/**********************************************************************************************************
*函 数 名: static void JS_ADC_GPIO_Config(void)
*功能说明: ADC引脚初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void JS_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    /*==========================左手摇杆通道初始化===========================*/
    //使能GPIO时钟
    RCC_AHB1PeriphClockCmd(JSL_X_GPIO_CLK|JSL_Y_GPIO_CLK,ENABLE);
    //配置IO口
    GPIO_InitStructure.GPIO_Pin = JSL_X_PIN|JSL_Y_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    //不上拉也不下拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOF,&GPIO_InitStructure);

    /*==========================右手摇杆通道初始化===========================*/
    //使能GPIO时钟
    RCC_AHB1PeriphClockCmd(JSR_X_GPIO_CLK|JSR_Y_GPIO_CLK,ENABLE);
    //配置IO口
    GPIO_InitStructure.GPIO_Pin = JSR_X_PIN|JSR_Y_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    //不上拉也不下拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOF,&GPIO_InitStructure);

    // /*==========================云台通道初始化===========================*/
    // //使能GPIO时钟
    // RCC_AHB1PeriphClockCmd(YUNTAI_GPIO_CLK,ENABLE);
    // //配置IO口
    // GPIO_InitStructure.GPIO_Pin = YUNTAI_PIN;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    // //不上拉也不下拉
    // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    // GPIO_Init(GPIOF,&GPIO_InitStructure);

}

/**********************************************************************************************************
*函 数 名: static void JS_ADC_Mode_Config(void)
*功能说明: 通道数据DMA读取初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void JS_ADC_Mode_Config(void)
{
	  DMA_InitTypeDef DMA_InitStructure;
	  ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
    // ------------------DMA Init 结构体参数 初始化--------------------------
    // ADC3使用DMA2，数据流0，通道2，这个是手册固定死的
    // 开启DMA时钟
    RCC_AHB1PeriphClockCmd(JS_ADC_DMA_CLK, ENABLE); 
	  // 外设基址为：ADC 数据寄存器地址
	  DMA_InitStructure.DMA_PeripheralBaseAddr = JS_ADC_DR_ADDR;	
    // 存储器地址，实际上是一个内部SRAM的变量	
	  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;  
    // 数据传输方向为外设到存储器	
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	  // 缓冲区大小为，指一次传输的数据量
	  DMA_InitStructure.DMA_BufferSize =  ADC_DATA;	
	  // 外设寄存器只有一个，地址不用递增
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // 存储器地址固定
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
    // 外设数据大小为半字，即两个字节 
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    //	存储器数据大小也为半字，跟外设数据大小相同
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	  // 循环传输模式
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    // 禁止DMA FIFO	，使用直连模式
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
    // FIFO 大小，FIFO模式禁止时，这个不用配置	
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	  // 选择 DMA 通道，通道存在于流中
    DMA_InitStructure.DMA_Channel = JS_ADC_DMA_CHANNEL; 
    //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	  DMA_Init(JS_ADC_DMA_STREAM, &DMA_InitStructure);
	  // 使能DMA流
    DMA_Cmd(JS_ADC_DMA_STREAM, ENABLE);
	
	  // 开启ADC时钟
	  RCC_APB2PeriphClockCmd(JS_ADC_CLK , ENABLE);
    // -------------------ADC Common 结构体 参数 初始化------------------------
	  // 独立ADC模式
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    // 时钟为fpclk x分频	
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    // 禁止DMA直接访问模式	
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    // 采样时间间隔	
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
    ADC_CommonInit(&ADC_CommonInitStructure);
	
    // -------------------ADC Init 结构体 参数 初始化--------------------------
	  ADC_StructInit(&ADC_InitStructure);
    // ADC 分辨率
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    // 扫描模式，多通道采集需要	
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
    // 连续转换	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
    //禁止外部边沿触发
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    //外部触发通道，本例子使用软件触发，此值随便赋值即可
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    //数据右对齐	
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //转换通道 1个
    ADC_InitStructure.ADC_NbrOfConversion =  ADC_DATA ;                                    
    ADC_Init(JS_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序和采样时间周期
  ADC_RegularChannelConfig(JS_ADC, JSR_Y_ADC_CHANNEL, 1, 
	                         ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(JS_ADC, JSR_X_ADC_CHANNEL, 2, 
	                         ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(JS_ADC, JSL_X_ADC_CHANNEL, 3, 
	                         ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(JS_ADC, JSL_Y_ADC_CHANNEL, 4, 
	                         ADC_SampleTime_3Cycles); 
//   ADC_RegularChannelConfig(JS_ADC, JSL_LOCK_ADC_CHANNEL, 5, 
// 	                         ADC_SampleTime_3Cycles); 


//   ADC_RegularChannelConfig(JS_ADC, YUNTAI_ADC_CHANNEL, 6, 
// 	                         ADC_SampleTime_3Cycles);                             

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(JS_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(JS_ADC, ENABLE);
	
	// 使能ADC
  ADC_Cmd(JS_ADC, ENABLE);  
  //开始adc转换，软件触发
  ADC_SoftwareStartConv(JS_ADC);
}

/**********************************************************************************************************
*函 数 名: void JS_ADC_Init(void)
*功能说明: ADC+DMA读取各通道数据初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void JS_ADC_Init(void)
{
	JS_ADC_GPIO_Config();
	JS_ADC_Mode_Config();
}

