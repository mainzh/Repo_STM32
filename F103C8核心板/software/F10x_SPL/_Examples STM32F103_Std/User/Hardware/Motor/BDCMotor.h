/**
  ******************************************************************************
  * 直流有刷电机BDCMotor
  ******************************************************************************
  *
  * 试验名称：PWM驱动直流电机
  * 硬件接线：电机驱动    PWMA——PA2    AIN2——PA5    AIN1——PA4    STBY——电源正极
            VM——ST-LINK 5V    VCC——电源正极    GND——电源负极    AO1——直流电机红线    AO2——直流电机黑线
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "BDCMotor.h"
#include "Key.h"

uint8_t KeyNum;    //定义用于接收按键键码的变量
int8_t Speed;    //定义速度变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    BDCMotor_Init(100, 20000);    //直流电机初始化
    Key_Init();    //按键初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Speed:");    //1行1列显示字符串Speed:
    
    while (1)
    {
        KeyNum = KeyStop_GetNum();    //获取按键键码
        if (KeyNum == 1)    //按键1按下
        {
            Speed += 20;    //速度变量自增20
            if (Speed > 100)    //速度变量超过100后
            {
                Speed = -100;    //速度变量变为-100
                //此操作会让电机旋转方向突然改变，可能会因供电不足而导致单片机复位
                //若出现了此现象，则应避免使用这样的操作
            }
        }
        BDCMotor_SetSpeed(Speed);    //设置直流电机的速度为速度变量
        OLED_ShowSignedNum(1, 7, Speed, 3);    //OLED显示速度变量
    }
}
  ******************************************************************************
  */
  
#ifndef __MOTOR_H
#define __MOTOR_H


#include "stm32f10x.h"                  // Device header


void BDCMotor_Init(uint32_t Reso, float Freq);
void BDCMotor_SetSpeed(int8_t Speed);

#endif
