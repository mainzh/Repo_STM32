/**
  ******************************************************************************
  * 舵机
  ******************************************************************************
  *
  * 试验名称：PWM驱动舵机
  * 硬件接线：舵机    PWM（橙）——PA8    5V（红）——ST-LINK 5V    GND（棕）——电源负极
  *          按键：一端接电源负极，一端接PB1引脚
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;    //定义用于接收键码的变量
float Angle;    //定义角度变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Servo_Init(20000, 50);    //舵机初始化
    Key_Init();    //按键初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Angle:");    //1行1列显示字符串Angle:
    
    while (1)
    {
        KeyNum = KeyStop_GetNum();    //获取按键键码
        if (KeyNum == 1)    //按键1按下
        {
            Angle += 30;    //角度变量自增30
            if (Angle > 180)    //角度变量超过180后
            {
                Angle = 0;    //角度变量归零
            }
        }
        Servo_SetAngle(Angle);    //设置舵机的角度为角度变量
        OLED_ShowNum(1, 7, Angle, 3);    //OLED显示角度变量
    }
}
  ******************************************************************************
  */
  
#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header

void Servo_Init(uint32_t Reso, float Freq);
void Servo_SetAngle(float Angle);

#endif
