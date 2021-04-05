#include "pwm.h"


//ADC转换的电压值通过DMA方式传到SRAM
 extern __IO uint16_t ADC_ConvertedValue[ ADC_DATA ];

//定义全局遥控器发送数组
uint8_t   transport_date[8];

uint16_t lx_pwm,ly_pwm,rx_pwm,ry_pwm;

//通道值微调器
FineTuneData_t FineTuneData;

/**********************************************************************************************************
*函 数 名: void PWN_Modulation(void)
*功能说明: 通道信号编码
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void PWN_Modulation(void)
{
    FineTuneData.pith = (-33);
    FineTuneData.roll = 41;
    FineTuneData.yaw = (-30);
    FineTuneData.throttle = 20;
    //printf("\r\n   pith  = %d us   roll  = %d us  yaw  = %d us   throttle  = %d us  \r\n",ADC_ConvertedValue[0],ADC_ConvertedValue[1],ADC_ConvertedValue[2],ADC_ConvertedValue[3]);

    ry_pwm =(uint16_t) (1050.0f + 900.0f * (((float)ADC_ConvertedValue[0] - jsr_y_l)/(jsr_y_h - jsr_y_l)));
    rx_pwm =(uint16_t) (1050.0f + 900.0f * ((jsr_x_h - (float)ADC_ConvertedValue[1])/(jsr_x_h - jsr_x_l)));

    lx_pwm =(uint16_t) (1050.0f + 900.0f * (((float)ADC_ConvertedValue[2] - jsl_x_l)/(jsl_x_h - jsl_x_l)));
    ly_pwm =(uint16_t) (1050.0f + 900.0f * ((jsl_y_h - (float)ADC_ConvertedValue[3])/(jsl_y_h - jsl_y_l)));

    //加入通道微调
    ry_pwm += FineTuneData.pith;
    rx_pwm += FineTuneData.roll;
    lx_pwm += FineTuneData.yaw;
    ly_pwm += FineTuneData.throttle;

    transport_date[0] = (ry_pwm >> 8)&0xFF;//pith H
    transport_date[1] = (ry_pwm)&0xFF;
    transport_date[2] = (rx_pwm >> 8)&0xFF;//roll H
    transport_date[3] = (rx_pwm)&0xFF;
    transport_date[4] = (lx_pwm >> 8)&0xFF;//yaw H
    transport_date[5] = (lx_pwm)&0xFF;
    transport_date[6] = (ly_pwm >> 8)&0xFF;//yaw H
    transport_date[7] = (ly_pwm)&0xFF;

}

static uint32_t tcount;
uint8_t armedflag = 0;//解锁标志, 0:未解锁  1:解锁
/**********************************************************************************************************
*函 数 名: void ArmedAuxiliary(void)
*功能说明: 解锁辅助判断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ArmedAuxiliary(void)
{

    if((lx_pwm < 1100)&&(ly_pwm < 1100)&&(rx_pwm > 1900)&&(ry_pwm < 1100))//判断外八字解锁
    {
        tcount ++;

        if((tcount > 130) && (armedflag == 0))
        {
            armedflag = 1;
            tcount = 0;
            //ChangeTIM3time(50000);
            //TIM3->ARR = 50000;//指示灯 1s刷新一次
        }
        else if((tcount > 130) && (armedflag == 1))
        {
            armedflag = 0;
            tcount = 0;
            //ChangeTIM3time(5000);
            //TIM3->ARR = 5000;//指示灯 1s刷新十次
        }
    }else tcount = 0;
}
/**********************************************************************************************************
*函 数 名: uint8_t Getarmedflag(void)
*功能说明: 获取辅助解锁标志位
*形    参: 无
*返 回 值: 辅助解锁标志位
**********************************************************************************************************/
uint8_t Getarmedflag(void)
{
    return armedflag;
}
