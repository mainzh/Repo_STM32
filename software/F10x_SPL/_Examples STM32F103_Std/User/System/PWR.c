/**
  ******************************************************************************
  * PWR
  ******************************************************************************
  *
  * 试验名称：修改主频
  * 硬件接线：
  * 程序现象：
  *
  * 程序设计：
  *
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
    OLED_Init();    //OLED初始化
    
    OLED_ShowString(1, 1, "SYSCLK:");    //显示静态字符串
    OLED_ShowNum(1, 8, SystemCoreClock, 8);    //显示SystemCoreClock变量
                                                //SystemCoreClock的值表示当前的系统主频频率
    
    while (1)
    {
        OLED_ShowString(2, 1, "Running");    //闪烁Running，指示当前主循环运行的快慢
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(500);
    }
}
  ******************************************************************************
  *
  * 试验名称：睡眠模式+串口发送+接收
  * 硬件接线：USB转串口    TXD——PA10    RXD——PA9    GND——电源负极    VCC与3V3引脚短接
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;    //定义用于接收串口数据的变量

int main(void)
{
    OLED_Init();    //OLED初始化
    OLED_ShowString(1, 1, "RxData:");    //显示静态字符串
    
    Serial_Init();    //串口初始化
    
    while (1)
    {
        if (Serial_GetRxFlag() == 1)    //检查串口接收数据的标志位
        {
            RxData = Serial_GetRxData();    //获取串口接收的数据
            Serial_SendByte(RxData);    //串口将收到的数据回传回去，用于测试
            OLED_ShowHexNum(1, 8, RxData, 2);    //显示串口接收的数据
        }
        
        OLED_ShowString(2, 1, "Running");    //OLED闪烁Running，指示当前主循环正在运行
        Delay_ms(100);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        __WFI();    //执行WFI指令，CPU睡眠，并等待中断唤醒
    }
}
  ******************************************************************************
  *
  * 试验名称：停止模式+对射式红外传感器计次
  * 硬件接线：对射式红外传感器    DO——PB14    GND——电源负极    VCC——电源正极
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    CountSensor_Init();    //计数传感器初始化
    
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    //开启PWR的时钟
                                                            //停止模式和待机模式一定要记得开启
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Count:");
    
    while (1)
    {
        OLED_ShowNum(1, 7, CountSensor_Get(), 5);    //OLED不断刷新显示CountSensor_Get的返回值
        
        OLED_ShowString(2, 1, "Running");    //OLED闪烁Running，指示当前主循环正在运行
        Delay_ms(100);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);    //STM32进入停止模式，并等待中断唤醒
        SystemInit();    //唤醒后，要重新配置时钟
    }
}
  ******************************************************************************
  *
  * 试验名称：待机模式+实时时钟
  * 硬件接线：PA0——引线    在需要时短接到电源3.3V    验证唤醒待机模式
  * 程序现象：
  *
  * 程序设计：
  *
#include "Delay.h"
#include "OLED.h"
#include "MyRTC.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    MyRTC_Init();    //RTC初始化
    
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    //开启PWR的时钟
                                                            //停止模式和待机模式一定要记得开启
    
    //显示静态字符串
    OLED_ShowString(1, 1, "CNT :");
    OLED_ShowString(2, 1, "ALR :");
    OLED_ShowString(3, 1, "ALRF:");
    
    //使能WKUP引脚
    PWR_WakeUpPinCmd(ENABLE);    //使能位于PA0的WKUP引脚，WKUP引脚上升沿唤醒待机模式
    
    //设定闹钟
    uint32_t Alarm = RTC_GetCounter() + 10;    //闹钟为唤醒后当前时间的后10s
    RTC_SetAlarm(Alarm);    //写入闹钟值到RTC的ALR寄存器
    OLED_ShowNum(2, 6, Alarm, 10);    //显示闹钟值
    
    while (1)
    {
        OLED_ShowNum(1, 6, RTC_GetCounter(), 10);    //显示32位的秒计数器
        OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);    //显示闹钟标志位
        
        OLED_ShowString(4, 1, "Running");    //OLED闪烁Running，指示当前主循环正在运行
        Delay_ms(100);
        OLED_ShowString(4, 1, "       ");
        Delay_ms(100);
        
        OLED_ShowString(4, 9, "STANDBY");    //OLED闪烁STANDBY，指示即将进入待机模式
        Delay_ms(1000);
        OLED_ShowString(4, 9, "       ");
        Delay_ms(100);
        
        OLED_Clear();    //OLED清屏，模拟关闭外部所有的耗电设备，以达到极度省电
        
        PWR_EnterSTANDBYMode();    //STM32进入停止模式，并等待指定的唤醒事件（WKUP上升沿或RTC闹钟）
        //待机模式唤醒后，程序会重头开始运行
    }
}
  ******************************************************************************
  */
