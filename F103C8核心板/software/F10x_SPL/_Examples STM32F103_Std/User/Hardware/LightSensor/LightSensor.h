/**
  ******************************************************************************
  * 光敏传感器
  ******************************************************************************
  *
  * 试验名称：光敏传感器控制LED灯翻转
  * 硬件接线：光敏传感器：DO——PB13    VCC——电源正极    GND——电源负极
  * 程序现象：
  *
  * 程序设计：
  *
#include "LightSensor.h"
#include "LED.h"


int main(void)
{
    // 模块初始化
    LightSensor_Init();    //按键初始化
    LED_Init();
    
    while (1)
    {
        if (LightSensor_Get() == 1)
        {
            LED_Turn();
        }
    }
}
  *
  ******************************************************************************
  */

#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H


#include "stm32f10x.h"                  // Device header

void LightSensor_Init(void);
uint8_t LightSensor_Get(void);

#endif
