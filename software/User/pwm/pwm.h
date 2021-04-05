#ifndef __PWM_H
#define __PWM_H

#include "stm32f4xx.h"
#include "adc.h"
#include "74hc165.h"
#include "delay.h"
#include "beeper.h"


typedef struct 
{
    int16_t yaw;
    int16_t pith;
    int16_t roll;
    int16_t throttle;

}FineTuneData_t;


#define jsr_y_h     3620.0f
#define jsr_y_l     450.0f

#define jsr_x_h     3550.0f
#define jsr_x_l     420.0f

#define jsl_x_h     3535.0f
#define jsl_x_l     500.0f

#define jsl_y_h     3520.0f
#define jsl_y_l     410.0f





void PWN_Modulation(void);
void ArmedAuxiliary(void);
uint8_t Getarmedflag(void);

#endif


