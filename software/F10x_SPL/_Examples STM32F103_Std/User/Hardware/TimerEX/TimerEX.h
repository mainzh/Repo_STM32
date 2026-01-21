/**
  ******************************************************************************
  * 定时器定时中断
  ******************************************************************************
  *
  * 试验名称：定时器外部时钟中断
  * 硬件接线：对射式红外传感器    DO——PA0    GND——电源负极    VCC——电源正极
  * 程序现象：遮挡一次对射式红外传感器，CNT加1，遮挡第10次时，Num加1
  *
  * 程序设计：
  *
#include "OLED.h"
#include "TimerEX.h"


uint16_t Num;    //定义在定时器中断里自增的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    TimerEX_Init();    //定时中断初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Num:");    //1行1列显示字符串Num:
    OLED_ShowString(2, 1, "CNT:");    //2行1列显示字符串CNT:
    
    while (1)
    {
        OLED_ShowNum(1, 5, Num, 5);    //不断刷新显示Num变量
        OLED_ShowNum(2, 5, Timer_GetCounter(), 5);    //不断刷新显示CNT的值
    }
}


void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        Num ++;    //Num变量自增，用于测试定时中断
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //清除TIM2更新事件的中断标志位
    }
}
  ******************************************************************************
  */

#ifndef __TIMEREX_H
#define __TIMEREX_H


#include "stm32f10x.h"                  // Device header

void TimerEX_Init(void);
uint16_t Timer_GetCounter(void);

#endif
