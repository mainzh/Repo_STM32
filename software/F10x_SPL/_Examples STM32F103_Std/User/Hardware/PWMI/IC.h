/**
  ******************************************************************************
  * TIM输入捕获
  ******************************************************************************
  *
  * 试验名称：输入捕获模式测频率及周期
  * 硬件接线：PA0——PA6
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    PWM_Init(100, 1000);    //PWM初始化
    IC_Init(65536, 15.3);    //输入捕获初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Freq:00000Hz");    //1行1列显示字符串Freq:00000Hz
    OLED_ShowString(2, 1, "Duty:00%");    //2行1列显示字符串Duty:00%
    
    //使用PWM模块提供输入捕获的测试信号
    PWM_SetPrescaler(7200 - 1);    //PWM频率Freq = 72M / (PSC + 1) / 100
    PWM_SetCompare1(50);    //PWM占空比Duty = CCR / 100
    
    while (1)
    {
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);    //不断刷新显示输入捕获测得的频率
        OLED_ShowNum(2, 6, IC_GetDuty(), 2);    //不断刷新显示输入捕获测得的占空比
    }
}
  *
  ******************************************************************************
  */
  
#ifndef __IC_H
#define __IC_H

#include "stm32f10x.h"                  // Device header

void IC_Init(uint32_t Reso, float Freq);
uint32_t IC_GetFreq(void);
uint32_t IC_GetDuty(void);

#endif
