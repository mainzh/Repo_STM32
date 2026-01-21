/**
  ******************************************************************************
  * main函数
  ******************************************************************************
  *
  * 试验名称：
  * 硬件接线：
  * 程序现象：
  *
  * 程序设计：
  */
#include "OLED.h"
#include "Timer.h"

uint16_t Num;    //定义在定时器中断里自增的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Timer_Init(1099, 1);    //定时中断初始化，分辨率1000，频率1
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Num:");    //1行1列显示字符串Num:
    
    while (1)
    {
        OLED_ShowNum(1, 5, Num, 5);    //不断刷新显示Num变量
    }
}

//TIM2中断函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        Num ++;    //Num变量自增，用于测试定时中断
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //清除TIM2更新事件的中断标志位
    }
}
