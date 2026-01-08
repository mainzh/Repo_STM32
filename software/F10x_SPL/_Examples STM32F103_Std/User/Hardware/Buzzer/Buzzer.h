/**
  ******************************************************************************
  * 蜂鸣器
  ******************************************************************************
  *
  * 试验名称：蜂鸣器周期性蜂鸣
  * 硬件接线：蜂鸣器：I/O——PB12    VCC——电源正极    GND——电源负极
  * 程序现象：
  *
  * 程序设计：
  *
#include "Buzzer.h"

uint8_t KeyNum;

int main(void)
{
    // 模块初始化
    Buzzer_Init();    //蜂鸣器初始化
    
    while (1)
    {
        Buzzer_ON();
        Delay_ms(100);
        Buzzer_OFF();
        Delay_ms(100);
    }
}
  *
  ******************************************************************************
  */

#ifndef __BUZZER_H
#define __BUZZER_H


#include "stm32f10x.h"                  // Device header

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Turn(void);

#endif
