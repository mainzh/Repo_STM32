/**
  ******************************************************************************
  * FLASH
  ******************************************************************************
  *
  * 试验名称：读取芯片ID
  * 硬件接线：
  * 程序现象：
  *
  * 程序设计：
  *
#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
    OLED_Init();    //OLED初始化
    
    OLED_ShowString(1, 1, "F_SIZE:");    //显示静态字符串
    OLED_ShowHexNum(1, 8, *((__IO uint16_t *)(0x1FFFF7E0)), 4);    //使用指针读取指定地址下的闪存容量寄存器
    
    OLED_ShowString(2, 1, "U_ID:");		//显示静态字符串
    OLED_ShowHexNum(2, 6, *((__IO uint16_t *)(0x1FFFF7E8)), 4);    //使用指针读取指定地址下的产品唯一身份标识寄存器
    OLED_ShowHexNum(2, 11, *((__IO uint16_t *)(0x1FFFF7E8 + 0x02)), 4);
    OLED_ShowHexNum(3, 1, *((__IO uint32_t *)(0x1FFFF7E8 + 0x04)), 8);
    OLED_ShowHexNum(4, 1, *((__IO uint32_t *)(0x1FFFF7E8 + 0x08)), 8);
    
    while (1)
    {
        
    }
}
  ******************************************************************************
  *
  * 试验名称：读写内部FLASH
  * 硬件接线：电源负极——按键1——PB1    电源负极——按键2——PB11
  * 程序现象：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "Store.h"
#include "Key.h"

uint8_t KeyNum;    //定义用于接收按键键码的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    Key_Init();    //按键初始化
    Store_Init();    //参数存储模块初始化，在上电的时候将闪存的数据加载回Store_Data，实现掉电不丢失
    
    //显示静态字符串
    OLED_ShowString(1, 1, "Flag:");
    OLED_ShowString(2, 1, "Data:");
    
    while (1)
    {
        KeyNum = Key_GetNum();    //获取按键键码
        
        if (KeyNum == 1)    //按键1按下
        {
            Store_Data[1] ++;    //变换测试数据
            Store_Data[2] += 2;
            Store_Data[3] += 3;
            Store_Data[4] += 4;
            Store_Save();    //将Store_Data的数据备份保存到闪存，实现掉电不丢失
        }
        
        if (KeyNum == 2)    //按键2按下
        {
            Store_Clear();    //将Store_Data的数据全部清0
        }
        
        OLED_ShowHexNum(1, 6, Store_Data[0], 4);    //显示Store_Data的第一位标志位
        OLED_ShowHexNum(3, 1, Store_Data[1], 4);    //显示Store_Data的有效存储数据
        OLED_ShowHexNum(3, 6, Store_Data[2], 4);
        OLED_ShowHexNum(4, 1, Store_Data[3], 4);
        OLED_ShowHexNum(4, 6, Store_Data[4], 4);
    }
}
  ******************************************************************************
  */

#ifndef __STORE_H
#define __STORE_H


#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

extern uint16_t Store_Data[];

void Store_Init(void);
void Store_Save(void);
void Store_Clear(void);

#endif
