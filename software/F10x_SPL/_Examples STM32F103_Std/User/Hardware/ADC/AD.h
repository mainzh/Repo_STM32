/**
  ******************************************************************************
  * ADC
  ******************************************************************************
  *
  * 试验名称：AD多通道
  * 硬件接线：电位器    中间引脚——PA0    左边引脚——电源负极    右边引脚——电源正极
  *          光敏传感器    AO——PA1    GND——电源负极    VCC——电源正极
  *          热敏传感器    AO——PA2    GND——电源负极    VCC——电源正极
  *          反射式红外传感器    AO——PA3    GND——电源负极    VCC——电源正极
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "OLED.h"
#include "AD.h"


float Voltage;    //定义电压变量

uint16_t AD0, AD1, AD2, AD3;    //定义AD值变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    AD_Init();    //AD初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "AD0:");
    OLED_ShowString(2, 1, "AD1:");
    OLED_ShowString(3, 1, "AD2:");
    OLED_ShowString(4, 1, "AD3:");
    
    while (1)
    {
        AD0 = AD_GetValue(ADC_Channel_0);    //单次启动ADC，转换通道0
        Voltage = (float)AD0 / 4095 * 3.3;    //将AD值线性变换到0~3.3的范围，表示电压
        
        AD1 = AD_GetValue(ADC_Channel_1);    //单次启动ADC，转换通道1
        AD2 = AD_GetValue(ADC_Channel_2);    //单次启动ADC，转换通道2
        AD3 = AD_GetValue(ADC_Channel_3);    //单次启动ADC，转换通道3
        
        OLED_ShowNum(1, 5, AD0, 4);    //显示通道0的转换结果AD0
        OLED_ShowNum(1, 9, Voltage, 1);    //显示电压值的整数部分
        OLED_ShowNum(1, 11, (uint16_t)(Voltage * 100) % 100, 2);    //显示电压值的小数部分
        
        OLED_ShowNum(2, 5, AD1, 4);    //显示通道1的转换结果AD1
        OLED_ShowNum(3, 5, AD2, 4);    //显示通道2的转换结果AD2
        OLED_ShowNum(4, 5, AD3, 4);    //显示通道3的转换结果AD3
        
        Delay_ms(100);    //延时100ms，手动增加一些转换的间隔时间
    }
}
  ******************************************************************************
  *
  * 试验名称：AD+DMA多通道
  * 硬件接线：电位器    中间引脚——PA0    左边引脚——电源负极    右边引脚——电源正极
  *          光敏传感器    AO——PA1    GND——电源负极    VCC——电源正极
  *          热敏传感器    AO——PA2    GND——电源负极    VCC——电源正极
  *          反射式红外传感器    AO——PA3    GND——电源负极    VCC——电源正极
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    AD_Init();    //AD初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "AD0:");
    OLED_ShowString(2, 1, "AD1:");
    OLED_ShowString(3, 1, "AD2:");
    OLED_ShowString(4, 1, "AD3:");
    
    while (1)
    {
        OLED_ShowNum(1, 5, AD_Value[0], 4);    //显示转换结果第0个数据
        OLED_ShowNum(2, 5, AD_Value[1], 4);    //显示转换结果第1个数据
        OLED_ShowNum(3, 5, AD_Value[2], 4);    //显示转换结果第2个数据
        OLED_ShowNum(4, 5, AD_Value[3], 4);    //显示转换结果第3个数据
        
        Delay_ms(100);    //延时100ms，手动增加一些转换的间隔时间
    }
}
  ******************************************************************************
  */

#ifndef __AD_H
#define __AD_H

#include "stm32f10x.h"                  // Device header

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);


extern uint16_t AD_Value[4];
void AD_DMA_Init(void);

#endif
