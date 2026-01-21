/**
  ******************************************************************************
  * MPU6050
  ******************************************************************************
  *
  * 试验名称：软件I2C读写MPU6050
  * 硬件接线：MPU6050    SCL——PB10    SDA——PB11    GND——电源负极    VCC——电源正极
  * 程序现象：OLED显示：
  *          1、设备ID
  *          2、左边三个数据是加速度传感器的输出数据 x、y、z轴的加速度
  *          3、右边三个数据是陀螺仪传感器的输出数据 x、y、z轴的角速度
  *
  * 程序设计：
  *
#include "OLED.h"
#include "MPU6050_S.h"

uint8_t ID;    //定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;    //定义用于存放各个数据的变量

int main(void)
{
    //模块初始化
    OLED_Init();    //OLED初始化
    MPU6050_S_Init();    //MPU6050初始化
    
    //显示ID号
    OLED_ShowString(1, 1, "ID:");    //显示静态字符串
    ID = MPU6050_S_GetID();    //获取MPU6050的ID号
    OLED_ShowHexNum(1, 4, ID, 2);    //OLED显示ID号
    
    while (1)
    {
        MPU6050_S_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);    //获取MPU6050的数据
        OLED_ShowSignedNum(2, 1, AX, 5);    //OLED显示数据
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
    }
}
  ******************************************************************************
  */

#ifndef __MPU6050_S_H
#define __MPU6050_S_H


#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

void MPU6050_S_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_S_ReadReg(uint8_t RegAddress);

void MPU6050_S_Init(void);
uint8_t MPU6050_S_GetID(void);
void MPU6050_S_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
