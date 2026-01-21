/**
  ******************************************************************************
  * LED发光二极管
  ******************************************************************************
  *          注：在输出模式下使用PC13至PC15的I/O口功能是有限制的：
  *              速度必须限制在2MHz以下，最大负载为30pF，而且这些I/O口绝对不能当作电流源(如驱动LED)
  *
  * 试验名称：LED灯闪烁
  * 硬件接线：单片机上LED负极——PC13
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "LED.h"


int main(void)
{
    // 模块初始化
    LED_Init();    //LED初始化
    
    while (1)
    {
        LED_ON();
        Delay_ms(500);
        LED_OFF();
        Delay_ms(500);
    }
}
  ******************************************************************************
  *
  * 试验名称：定时器实现非阻塞式程序 按键控制LED
  * 硬件接线：单片机上LED负极——PC13
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Timer.h"

uint8_t KeyNum;    //全局变量，获取按键键码
uint8_t LEDMode;    //标志位

uint16_t i;    //全局变量，自增，指示主循环是否在快速的刷新运行

int main(void)
{
    // 模块初始化
    OLED_Init();
    LED_Init();
    Key_Init();    //按键初始化
    Timer_Init(1000, 1000);    //定时中断初始化，分辨率1000，频率1000
    
    OLED_ShowString(1, 1, "main:");
    OLED_ShowString(2, 1, "LEDMode:");
    
    while (1)
    {
        KeyNum = Key_GetNum();    //非阻塞式
        if (KeyNum == 1)
        {
            LEDMode ++;
            LEDMode %= 5;    //按键每次按下，标志位LEDMode自增，到4后清零
            LED_SetMode(LEDMode);
        }
        
        OLED_ShowNum(1, 6, i ++, 5);
        OLED_ShowNum(2, 9, LEDMode, 1);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        // 用户程序
        Key_Tick();
        LED_Tick();
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    // 清除TIM2更新事件的中断标志位
    }
}

注意：定时中断被多个模块复用，各模块的代码执行时间不能太久，不要出现一个中断还没结束，下一个中断就来了（中断重叠）
      判断中断重叠：进中断先清中断标志位，中断退出之前再查看中断标志位，若还没置1，则中断没有重叠，若置1，则中断中代码执行过久，中断重叠
      改进方法：1、减小模块内中断函数的内容，减小执行时间
               2、增加定时器的通用时间基准，一般单片机使用1ms进一次中断，可以改成2ms进一次中断
  ******************************************************************************
  */
  
#ifndef __LED_H
#define __LED_H


#include "stm32f10x.h"                  // Device header


void LED_Init(void);


void LED_SetMode(uint8_t Mode);
void LED_Tick(void);


void LED_ON(void);
void LED_OFF(void);
void LED_Turn(void);


#endif
