/**
  ******************************************************************************
  * RTC
  ******************************************************************************
  *
  * 试验名称：读写BKP备份寄存器
  * 硬件接线：电源负极——按键——PB1
  *          ST-LINK   3.3V——VBAT
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Key.h"

uint8_t KeyNum;    //定义用于接收按键键码的变量

uint16_t ArrayWrite[] = {0x1234, 0x5678};    //定义要写入数据的测试数组
uint16_t ArrayRead[2];    //定义要读取数据的测试数组

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Key_Init();    //按键初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "W:");
    OLED_ShowString(2, 1, "R:");
    
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    //开启PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);    //开启BKP的时钟
    
    //备份寄存器访问使能
    PWR_BackupAccessCmd(ENABLE);    //使用PWR开启对备份寄存器的访问
    
    while (1)
    {
        KeyNum = Key_GetNum();    //获取按键键码
        
        if (KeyNum == 1)    //按键1按下
        {
            ArrayWrite[0] ++;    //测试数据自增
            ArrayWrite[1] ++;
            
            BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);    //写入测试数据到备份寄存器
            BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
            
            OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);    //显示写入的测试数据
            OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
        }
        
        ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);    //读取备份寄存器的数据
        ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
        
        OLED_ShowHexNum(2, 3, ArrayRead[0], 4);    //显示读取的备份寄存器数据
        OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
    }
}
  ******************************************************************************
  *
  * 试验名称：RTC实时时钟
  * 硬件接线：ST-LINK   3.3V——VBAT
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    MyRTC_Init();    //RTC初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
    OLED_ShowString(2, 1, "Time:XX:XX:XX");
    OLED_ShowString(3, 1, "CNT :");
    OLED_ShowString(4, 1, "DIV :");
    
    while (1)
    {
        MyRTC_ReadTime();    //RTC读取时间，最新的时间存储到MyRTC_Time数组中
        
        OLED_ShowNum(1, 6, MyRTC_Time[0], 4);    //显示MyRTC_Time数组中的时间值，年
        OLED_ShowNum(1, 11, MyRTC_Time[1], 2);    //月
        OLED_ShowNum(1, 14, MyRTC_Time[2], 2);    //日
        OLED_ShowNum(2, 6, MyRTC_Time[3], 2);    //时
        OLED_ShowNum(2, 9, MyRTC_Time[4], 2);    //分
        OLED_ShowNum(2, 12, MyRTC_Time[5], 2);    //秒
        
        OLED_ShowNum(3, 6, RTC_GetCounter(), 10);    //显示32位的秒计数器
        OLED_ShowNum(4, 6, RTC_GetDivider(), 10);    //显示余数寄存器
    }
}
  ******************************************************************************
  */

#ifndef __MYRTC_H
#define __MYRTC_H


#include "stm32f10x.h"                  // Device header
#include <time.h>

extern uint16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
