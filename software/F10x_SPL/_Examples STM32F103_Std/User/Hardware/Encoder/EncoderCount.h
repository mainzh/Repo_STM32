/**
  ******************************************************************************
  * 旋转编码器
  ******************************************************************************
  *
  * 试验名称：旋转编码器计次
  * 硬件接线：旋转编码器    A——PB0    B——PB1    GND——电源负极    VCC——电源正极
  * 程序现象：转动旋转编码器，正转数字增加，反转数字减小
  *
  * 程序设计：
  *
#include "OLED.h"
#include "EncoderCount.h"

int16_t Num;    //定义待被旋转编码器调节的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    EncoderCount_Init();    //旋转编码器计次初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Num:");    //1行1列显示字符串Num:
    
    while (1)
    {
        Num += EncoderCount_Get();    //获取自上此调用此函数后，旋转编码器的增量值，并将增量值加到Num上
        OLED_ShowSignedNum(1, 5, Num, 5);    //显示Num
    }
}
  *
  ******************************************************************************
  */

#ifndef __ENCODER_H
#define __ENCODER_H


#include "stm32f10x.h"                  // Device header


void EncoderCount_Init(void);
int16_t EncoderCount_Get(void);

#endif
