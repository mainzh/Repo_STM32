#include "LightSensor.h"


/*引脚配置*/
#define LightSensor_Pin GPIO_Pin_13
#define LightSensor_GPIO_Port GPIOB

/**
  * 函    数：光敏传感器初始化
  * 参    数：无
  * 返 回 值：无
  */
void LightSensor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LightSensor_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(LightSensor_GPIO_Port, &GPIO_InitStructure);
}

/**
  * 函    数：获取当前光敏传感器输出的高低电平
  * 参    数：无
  * 返 回 值：光敏传感器输出的高低电平，范围：0/1
  */
uint8_t LightSensor_Get(void)
{
    return GPIO_ReadInputDataBit(LightSensor_GPIO_Port, LightSensor_Pin);    /* 返回GPIO输入寄存器的状态 */
}
