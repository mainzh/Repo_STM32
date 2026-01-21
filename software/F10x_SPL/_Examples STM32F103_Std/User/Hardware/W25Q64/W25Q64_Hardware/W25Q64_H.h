/**
  ******************************************************************************
  * W25Q64
  ******************************************************************************
  *
  * 试验名称：硬件SPI读写W25Q64
  * 硬件接线：W25Q64    CS——PA4    CLK——PA5    DO——PA6    DI——PA7    GND——电源负极    VCC——电源正极
  * 程序现象：OLED显示：
  *              厂商MID    设备DID
  *              写入数据    W：
  *              读取数据    R：
  *
  * 程序设计：
  *
#include "OLED.h"
#include "W25Q64_H.h"

uint8_t MID;    //定义用于存放MID号的变量
uint16_t DID;    //定义用于存放DID号的变量

uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};    //定义要写入数据的测试数组
uint8_t ArrayRead[4];    //定义要读取数据的测试数组

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    W25Q64_H_Init();    //W25Q64初始化
    
    //显示静态字符串
    OLED_ShowString(1, 1, "MID:   DID:");
    OLED_ShowString(2, 1, "W:");
    OLED_ShowString(3, 1, "R:");
    
    //显示ID号
    W25Q64_H_ReadID(&MID, &DID);    //获取W25Q64的ID号
    OLED_ShowHexNum(1, 5, MID, 2);    //显示MID
    OLED_ShowHexNum(1, 12, DID, 4);    //显示DID
    
    //W25Q64功能函数测试
    W25Q64_H_SectorErase(0x000000);    //扇区擦除
    W25Q64_H_PageProgram(0x000000, ArrayWrite, 4);    //将写入数据的测试数组写入到W25Q64中
    
    W25Q64_H_ReadData(0x000000, ArrayRead, 4);    //读取刚写入的测试数据到读取数据的测试数组中
    
    //显示数据
    OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);    //显示写入数据的测试数组
    OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
    OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
    OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
    
    OLED_ShowHexNum(3, 3, ArrayRead[0], 2);    //显示读取数据的测试数组
    OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
    OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
    OLED_ShowHexNum(3, 12, ArrayRead[3], 2);
    
    while (1)
    {
        
    }
}
  ******************************************************************************
  */

#ifndef __W25Q64_H_H
#define __W25Q64_H_H


#include "stm32f10x.h"                  // Device header
#include "MySPI_H.h"
#include "W25Q64_Ins.h"

void W25Q64_H_Init(void);
void W25Q64_H_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_H_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_H_SectorErase(uint32_t Address);
void W25Q64_H_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);

#endif
