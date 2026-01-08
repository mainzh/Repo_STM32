#include "Buzzer.h"


/*引脚配置*/
#define Buzzer_Pin GPIO_Pin_12
#define Buzzer_GPIO_Port GPIOB


/**
  * 函    数：蜂鸣器初始化
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStructure);
    
    /*设置GPIO初始化后的默认电平*/
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);
}

/**
  * 函    数：蜂鸣器开启
  * 参    数：无  （根据硬件）
  * 返 回 值：无
  */
void Buzzer_ON(void)
{
    GPIO_ResetBits(Buzzer_GPIO_Port, Buzzer_Pin);    /* 设置引脚为低电平 */
}

/**
  * 函    数：蜂鸣器关闭
  * 参    数：无  （根据硬件）
  * 返 回 值：无
  */
void Buzzer_OFF(void)
{
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);    /* 设置引脚为高电平 */
}

/**
  * 函    数：蜂鸣器状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void Buzzer_Turn(void)
{
    if (GPIO_ReadOutputDataBit(Buzzer_GPIO_Port, Buzzer_Pin) == 0)    /* 获取输出寄存器的状态，如果当前引脚输出低电平 */
    {
        GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);    /* 则设置PB12引脚为高电平 */
    }
    else    /* 否则，即当前引脚输出高电平 */
    {
        GPIO_ResetBits(Buzzer_GPIO_Port, Buzzer_Pin);    /* 则设置PB12引脚为低电平 */
    }
}
