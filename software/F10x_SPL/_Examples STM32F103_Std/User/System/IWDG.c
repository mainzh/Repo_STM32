/**
  ******************************************************************************
  * IWDG
  ******************************************************************************
  *
  * 试验名称：IWDG独立看门狗
  * 硬件接线：电源负极——按键——PB1
  * 程序现象：
  *
  * 程序设计：
  *
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Key_Init();    //按键初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "IWDG TEST");
    
    //判断复位信号来源
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)    //如果是独立看门狗复位
    {
        OLED_ShowString(2, 1, "IWDGRST");    //OLED闪烁IWDGRST字符串
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        RCC_ClearFlag();    //清除标志位
    }
    else    //否则，即为其他复位
    {
        OLED_ShowString(3, 1, "RST");    //OLED闪烁RST字符串
        Delay_ms(500);
        OLED_ShowString(3, 1, "   ");
        Delay_ms(100);
    }
    
    //IWDG初始化
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);    //独立看门狗写使能
    IWDG_SetPrescaler(IWDG_Prescaler_16);    //设置预分频为16
    IWDG_SetReload(2499);    //设置重装值为2499，独立看门狗的超时时间为1000ms
    IWDG_ReloadCounter();    //重装计数器，喂狗
    IWDG_Enable();    //独立看门狗使能
    
    while (1)
    {
        Key_GetNum();    //调用阻塞式的按键扫描函数，模拟主循环卡死
        
        IWDG_ReloadCounter();    //重装计数器，喂狗
        
        OLED_ShowString(4, 1, "FEED");    //OLED闪烁FEED字符串
        Delay_ms(200);    //喂狗间隔为200+600=800ms
        OLED_ShowString(4, 1, "    ");
        Delay_ms(600);
    }
}
  ******************************************************************************
  *
  * 试验名称：WWDG窗口看门狗
  * 硬件接线：电源负极——按键——PB1
  * 程序现象：
  *
  * 程序设计：
  *
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Key_Init();    //按键初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "WWDG TEST");
    
    //判断复位信号来源
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)    //如果是窗口看门狗复位
    {
        OLED_ShowString(2, 1, "WWDGRST");    //OLED闪烁WWDGRST字符串
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        RCC_ClearFlag();    //清除标志位
    }
    else    //否则，即为其他复位
    {
        OLED_ShowString(3, 1, "RST");    //OLED闪烁RST字符串
        Delay_ms(500);
        OLED_ShowString(3, 1, "   ");
        Delay_ms(100);
    }
    
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);    //开启WWDG的时钟
    
    //WWDG初始化
    WWDG_SetPrescaler(WWDG_Prescaler_8);    //设置预分频为8
    WWDG_SetWindowValue(0x40 | 21);    //设置窗口值，窗口时间为30ms
    WWDG_Enable(0x40 | 54);    //使能并第一次喂狗，超时时间为50ms
    
    while (1)
    {
        Key_GetNum();    //调用阻塞式的按键扫描函数，模拟主循环卡死
        
        OLED_ShowString(4, 1, "FEED");    //OLED闪烁FEED字符串
        Delay_ms(20);    //喂狗间隔为20+20=40ms
        OLED_ShowString(4, 1, "    ");
        Delay_ms(20);
        
        WWDG_SetCounter(0x40 | 54);    //重装计数器，喂狗
    }
}
  ******************************************************************************
  */
