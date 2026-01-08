/**
  ******************************************************************************
  * 按键
  ******************************************************************************
  *
  * 试验名称：按键控制LED灯翻转
  * 硬件接线：按键：一端接电源负极，一端接PB1引脚
  * 程序现象：
  *
  * 程序设计：
  *
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
    // 模块初始化
    Key_Init();    //按键初始化
    
    while (1)
    {
        KeyNum = KeyStop_GetNum();
        if (KeyNum == 1)
        {
            LED_Turn();
        }
    }
}
  ******************************************************************************
  *
  * 试验名称：阻塞式程序的弊端
  * 硬件接线：按键：一端接电源负极，一端接PB1引脚
  * 程序现象：
  *          1、编译下载后，i在快速刷新自增，表示主循环在快速运行
  *          2、按下按键不松手，i暂停，表示程序阻塞在 等待按键松手的地方
  *          3、松开按键，LED闪烁，i在慢速自增，表示主循环受到LED闪烁时的阻塞 影响，每隔1s刷新一次
  *          4、再按下按键，LED没有关闭，按键不灵敏
  *          5、长按按键，LED才关闭，i在快速刷新自增，表示主循环在快速运行
  *
  * 程序设计：
  *
#include "OLED.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;    //全局变量，获取按键键码
uint8_t FlashFlag;    //标志位

uint16_t i;    //全局变量，自增，指示主循环是否在快速的刷新运行

int main(void)
{
    // 模块初始化
    OLED_Init();
    LED_Init();
    Key_Init();    //按键初始化
    
    while (1)
    {
        KeyNum = KeyStop_GetNum();    //阻塞式
        if (KeyNum == 1)
        {
            FlashFlag = !FlashFlag;    //按键每次按下，标志位取反，在0 1 0 1 之间切换
        }
        
        if (FlashFlag)    //标志位为1时，执行LED闪烁
        {
            LED_ON();
            Delay_ms(500);
            LED_OFF();
            Delay_ms(500);
        }
        else    //标志位为0时，执行LED关闭
        {
            LED_OFF();
        }
        
        OLED_ShowNum(1, 1, i ++, 5);
    }
}
  ******************************************************************************
  *
  * 试验名称：定时器实现非阻塞式程序 按键
  * 硬件接线：按键：一端接电源负极，一端接PB1引脚
  * 程序现象：
  *          
  *
  * 程序设计：
  *
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Timer.h"

uint8_t KeyNum;    //全局变量，获取按键键码
uint8_t FlashFlag;    //标志位

uint16_t i;    //全局变量，自增，指示主循环是否在快速的刷新运行

int main(void)
{
    // 模块初始化
    OLED_Init();
    LED_Init();
    Key_Init();    //按键初始化
    Timer_Init(1000, 1000);    //定时中断初始化，分辨率1000，频率1000
    
    while (1)
    {
        KeyNum = Key_GetNum();    //非阻塞式
        if (KeyNum == 1)
        {
            FlashFlag = !FlashFlag;    //按键每次按下，标志位取反，在0 1 0 1 之间切换
        }
        
        if (FlashFlag)    //标志位为1时，执行LED闪烁
        {
            LED_ON();
            Delay_ms(500);
            LED_OFF();
            Delay_ms(500);
        }
        else    //标志位为0时，执行LED关闭
        {
            LED_OFF();
        }
        
        OLED_ShowNum(1, 1, i ++, 5);
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        // 用户程序
        Key_Tick();
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    // 清除TIM2更新事件的中断标志位
    }
}
  ******************************************************************************
  */
  
#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"                  // Device header
#include "Delay.h"


void Key_Init(void);


uint8_t Key_GetNum(void);
void Key_Tick(void);


uint8_t KeyStop_GetNum(void);

#endif
