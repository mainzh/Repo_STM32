/**
  ******************************************************************************
  * PWM输出
  ******************************************************************************
  *
  * 试验名称：PWM驱动LED呼吸灯
  * 硬件接线：LED  正极——PA0    负极——电源负极
  *                     启用引脚重映射功能后，LED正极接PA15
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "PWM.h"


uint8_t i;    //定义for循环的变量

int main(void)
{
    //模块初始化
    PWM_Init(100, 1000);    //PWM初始化
    
    while (1)
    {
        for (i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(i);    //依次将定时器的CCR寄存器设置为0~100，PWM占空比逐渐增大，LED逐渐变亮
            Delay_ms(10);    //延时10ms
        }
        for (i = 0; i <= 100; i++)
        {
            PWM_SetCompare1(100 - i);    //依次将定时器的CCR寄存器设置为100~0，PWM占空比逐渐减小，LED逐渐变暗
            Delay_ms(10);    //延时10ms
        }
    }
}
  *
  ******************************************************************************
  */
  
#ifndef __PWM_H
#define __PWM_H


#include "stm32f10x.h"                  // Device header

void PWM_Init(uint32_t Reso, float Freq);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetPrescaler(uint16_t Prescaler);

#endif
