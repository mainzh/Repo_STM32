/**
  ******************************************************************************
  * 串口通信
  ******************************************************************************
  *
  * 试验名称：串口发送
  * 硬件接线：
  * 程序现象：
  *
  * 程序设计：
  *
#include "Serial.h"

int main(void)
{
    //模块初始化
    
    Serial_Init();    //串口初始化
    
    //串口基本函数
    Serial_SendByte(0x41);    //串口发送一个字节数据0x41
    
    uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};    //定义数组
    Serial_SendArray(MyArray, 4);    //串口发送一个数组
    
    Serial_SendString("\r\nNum1=");    //串口发送字符串
    
    Serial_SendNumber(111, 3);    //串口发送数字
    
    //下述3种方法可实现printf的效果
    
//    //方法1：直接重定向printf，但printf函数只有一个，此方法不能在多处使用
//    printf("\r\nNum2=%d", 222);    //串口发送printf打印的格式化字符串
//        //需要重定向fputc函数，并在工程选项里勾选Use MicroLIB
    
//    //方法2：使用sprintf打印到字符数组，再用串口发送字符数组，此方法打印到字符数组，之后想怎么处理都可以，可在多处使用
//    char String[100];    //定义字符数组
//    sprintf(String, "\r\nNum3=%d", 333);    //使用sprintf，把格式化字符串打印到字符数组
//    Serial_SendString(String);    //串口发送字符数组（字符串）
    
    //方法3：将sprintf函数封装起来，实现专用的printf，此方法就是把方法2封装起来，更加简洁实用，可在多处使用
    Serial_Printf("\r\nNum4=%d", 444);    //串口打印字符串，使用自己封装的函数实现printf的效果
    Serial_Printf("\r\n");
    
    while (1)
    {
        
    }
}
  ******************************************************************************
  *
  * 试验名称：串口发送+接收
  * 硬件接线：
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;    //定义用于接收串口数据的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Serial_Init();    //串口初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "RxData:");
    
    //串口发送 提示信息
    Serial_SendString("请将串口助手接收区、发送区设置为 HEX模式\r\n");
    Serial_SendString("请在发送区输入一个字节的数据，不需以 0x开头，直接输入16进制数据，点击 发送\r\n");
    Serial_SendString("接收区 显示\r\n");
    
    while (1)
    {
        if (Serial_GetRxFlag() == 1)    //检查串口接收数据的标志位
        {
            RxData = Serial_GetRxData();    //获取串口接收的数据
            Serial_SendByte(RxData);    //串口将收到的数据回传回去，用于测试
            OLED_ShowHexNum(1, 8, RxData, 2);    //显示串口接收的数据
        }
    }
}
  ******************************************************************************
  *
  * 试验名称：串口收发HEX数据包
  * 硬件接线：USB转串口    TXD——PA10    RXD——PA9    GND——电源负极    VCC与3V3引脚短接
  *          电源负极——按键——PB1
  * 程序现象：1、请将串口助手接收区、发送区设置为 HEX模式；
  *          2、将在 接收区 接收到 初始化的 数据包；
  *          3、请在 发送区 输入 一个数据包 ，以 FF 为 包头，4个16进制字节 为载荷数据，以 FE 为 包尾，不需要用 空格 间隔；
  *          4、将在 接收区 显示输入的 数据包
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

uint8_t KeyNum;    //定义用于接收按键键码的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Key_Init();    //按键初始化
    Serial_Init();    //串口初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "TxPacket");
    OLED_ShowString(3, 1, "RxPacket");
    
    //设置发送数据包数组的初始值，用于测试
    Serial_TxPacket[0] = 0x01;
    Serial_TxPacket[1] = 0x02;
    Serial_TxPacket[2] = 0x03;
    Serial_TxPacket[3] = 0x04;
    
    while (1)
    {
        KeyNum = Key_GetNum();    //获取按键键码
        if (KeyNum == 1)    //按键1按下
        {
            Serial_TxPacket[0] ++;    //测试数据自增
            Serial_TxPacket[1] ++;
            Serial_TxPacket[2] ++;
            Serial_TxPacket[3] ++;
            
            Serial_SendPacket();    //串口发送数据包Serial_TxPacket
            
            OLED_ShowHexNum(2, 1, Serial_TxPacket[0], 2);    //显示发送的数据包
            OLED_ShowHexNum(2, 4, Serial_TxPacket[1], 2);
            OLED_ShowHexNum(2, 7, Serial_TxPacket[2], 2);
            OLED_ShowHexNum(2, 10, Serial_TxPacket[3], 2);
        }
        
        if (Serial_GetRxFlag() == 1)    //如果接收到数据包
        {
            Serial_SendArray(Serial_RxPacket, 4);
            OLED_ShowHexNum(4, 1, Serial_RxPacket[0], 2);    //显示接收的数据包
            OLED_ShowHexNum(4, 4, Serial_RxPacket[1], 2);
            OLED_ShowHexNum(4, 7, Serial_RxPacket[2], 2);
            OLED_ShowHexNum(4, 10, Serial_RxPacket[3], 2);
        }
    }
}
  ******************************************************************************
  *
  * 试验名称：串口收发文本数据包
  * 硬件接线：USB转串口    TXD——PA10    RXD——PA9    GND——电源负极    VCC与3V3引脚短接
  *          电源负极——按键——PB1
  * 程序现象：1、请将串口助手接收区、发送区设置为 HEX模式；
  *          2、将在 接收区 接收到 初始化的 数据包；
  *          3、请在 发送区 输入 一个数据包 ，以 FF 为 包头，4个16进制字节 为载荷数据，以 FE 为 包尾，不需要用 空格 间隔；
  *          4、将在 接收区 显示输入的 数据包
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    LED_Init();    //LED初始化
    Serial_Init();    //串口初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "TxPacket");
    OLED_ShowString(3, 1, "RxPacket");
    
    //串口发送 提示信息
    Serial_SendString("请将串口助手接收区、发送区设置为 文本模式\r\n");
    Serial_SendString("请在发送区输入指令，以 @ 开始，以 回车 结束，点击 发送\r\n");
    Serial_SendString("LED_ON 控制 LED开，并在接收区 显示 LED_ON_OK\r\n");
    Serial_SendString("LED_OFF 控制 LED关，并在接收区 显示 LED_OFF_OK\r\n");
    Serial_SendString("若输入 其他未知指令，将在接收区 显示 ERROR_COMMAND\r\n");
    
    while (1)
    {
        if (Serial_RxFlag == 1)    //如果接收到数据包
        {
            OLED_ShowString(4, 1, "                ");
            OLED_ShowString(4, 1, Serial_RxPacketTXT);    //OLED清除指定位置，并显示接收到的数据包
            
            //将收到的数据包与预设的指令对比，以此决定将要执行的操作
            if (strcmp(Serial_RxPacketTXT, "LED_ON") == 0)    //如果收到LED_ON指令
            {
                LED1_ON();    //点亮LED
                Serial_SendString("LED_ON_OK\r\n");    //串口回传一个字符串LED_ON_OK
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_ON_OK");    //OLED清除指定位置，并显示LED_ON_OK
            }
            else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)    //如果收到LED_OFF指令
            {
                LED1_OFF();    //熄灭LED
                Serial_SendString("LED_OFF_OK\r\n");    //串口回传一个字符串LED_OFF_OK
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_OFF_OK");    //OLED清除指定位置，并显示LED_OFF_OK
            }
            else    //上述所有条件均不满足，即收到了未知指令
            {
                Serial_SendString("ERROR_COMMAND\r\n");    //串口回传一个字符串ERROR_COMMAND
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "ERROR_COMMAND");    //OLED清除指定位置，并显示ERROR_COMMAND
            }
            
            Serial_RxFlag = 0;    //处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
        }
    }
}
  ******************************************************************************
  */

#ifndef __SERIAL_H
#define __SERIAL_H


#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);


uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
void Serial_SendPacket(void);


extern uint8_t Serial_RxFlag;
extern char Serial_RxPacketTXT[];

#endif
