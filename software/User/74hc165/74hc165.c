#include "74hc165.h"

//定义HC165接收值
 uint8_t HC_Recieve_Date = 0;
 
/**********************************************************************************************************
*函 数 名: void HC165_GPIO_Config(void)
*功能说明: 配置74hc165相关PL,CP,Q7引脚
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void HC165_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 开启按键端口的时钟 */
    RCC_AHB1PeriphClockCmd(HC_PL_PIN|HC_CP_PIN|HC_Q7_PIN,ENABLE);

    //选择引脚
    GPIO_InitStructure.GPIO_Pin = HC_PL_PIN|HC_CP_PIN;
    //设置为输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    //设置为开漏输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //设置引脚速度
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    //设置上拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init(GPIOE,&GPIO_InitStructure);
    //GPIO_SetBits(GPIOE,hc_pl_gpio_pin|hc_cp_gpio_pin);

    //选择引脚
    GPIO_InitStructure.GPIO_Pin = HC_Q7_PIN;
    //设置为输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    //设置为推挽输出
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    //设置引脚速度
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    //设置上拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    
    GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/**********************************************************************************************************
*函 数 名: uint8_t HC165_Scan(void)
*功能说明: 74HC165扫描各个按键状态，并返回一个8位数据
*形    参: 无
*返 回 值: uint8_t temp
**********************************************************************************************************/
uint8_t HC165_Scan(void)
{
    uint8_t i,temp=0;
    HC_PL_1;
    HC_PL_0;
    HC_PL_1;

    if(GPIO_ReadInputDataBit(HC_Q7_GPIO_PORT,HC_Q7_PIN) == 1)     temp |= 0x01;

    for ( i = 0; i < 7; i++)
    {
        HC_CP_0;
        HC_CP_1;

        temp <<= 1;//temp左移一位，给新数据预留位置

        if(GPIO_ReadInputDataBit(HC_Q7_GPIO_PORT,HC_Q7_PIN) == 1)   temp |= 0x01;
        
        HC_CP_0;

    }
    HC_Recieve_Date  = temp;

    return temp ;    
}

/**********************************************************************************************************
*函 数 名: uint8_t BUTTON_Scan( uint8_t KEY)
*功能说明: 按键扫描程序
*形    参: 扫描按键数值对应的按键宏	
*返 回 值: 按键状态
**********************************************************************************************************/
uint8_t BUTTON_Scan( uint8_t KEY)
{
    uint8_t temp,i;
    temp = HC_Recieve_Date;
    if( temp == KEY )
    { 
        for ( i = 0; i < 30; i++)
        {
        } 
        return DOWN;
    }else
        return UP;
}


