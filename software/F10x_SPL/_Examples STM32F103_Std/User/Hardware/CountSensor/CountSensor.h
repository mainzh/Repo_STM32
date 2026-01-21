/**
  ******************************************************************************
  * 计数传感器
  ******************************************************************************
  *
  * 试验名称：对射式红外传感器外部中断计次
  * 硬件接线：对射式红外传感器    DO——PB14    GND——电源负极    VCC——电源正极
  * 程序现象：用不透光物体遮挡对射式红外传感器，计数+1
  *
  * 程序设计：
  *
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
    // 模块初始化
    OLED_Init();    //OLED初始化
    CountSensor_Init();    //计数传感器初始化
    
    // 显示静态字符串
    OLED_ShowString(1, 1, "Count:");    //OLED1行1列显示字符串 Count:
    
    while (1)
    {
        OLED_ShowNum(1, 7, CountSensor_Get(), 5);    //OLED1行7列显示CountSensor_Get的返回值
    }
}
  *
  ******************************************************************************
  */

#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H


#include "stm32f10x.h"                  // Device header

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
