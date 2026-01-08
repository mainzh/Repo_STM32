/**
  ******************************************************************************
  * 旋转编码器
  ******************************************************************************
  *
  * 试验名称：旋转编码器测速
  * 硬件接线：旋转编码器    A——PA6    B——PA7   GND——电源负极    VCC——电源正极
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Timer.h"
#include "EncoderSpeed.h"

int16_t Speed;    //定义速度变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Timer_Init(10000, 1);    //定时器初始化
    EncoderSpeed_Init();    //旋转编码器测速初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Speed:");    //1行1列显示字符串Speed:
    
    while (1)
    {
        OLED_ShowSignedNum(1, 7, Speed, 5);    //不断刷新显示编码器测得的最新速度
    }
}


void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        Speed = EncoderSpeed_Get();    //每隔固定时间段读取一次编码器计数增量值，即为速度值
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //清除TIM2更新事件的中断标志位
    }
}
  ******************************************************************************
  */

#ifndef __ENCODERSPEED_H
#define __ENCODERSPEED_H


#include "stm32f10x.h"                  // Device header

void EncoderSpeed_Init(void);
int16_t EncoderSpeed_Get(void);

#endif
