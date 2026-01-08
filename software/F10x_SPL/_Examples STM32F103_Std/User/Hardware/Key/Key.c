#include "Key.h"


#define Key_Pin GPIO_Pin_1
#define Key_GPIO_Port GPIOB


uint8_t Key_Num;    //全局变量，按键的键码值


/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = Key_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(Key_GPIO_Port, &GPIO_InitStructure);
}

/****************************************************************************************************************************/

/**
  * 函    数：获取按键键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~1，返回0代表按键按开，返回1代表按键1按下
  * 
  * （读后清零）
  */
uint8_t Key_GetNum(void)
{
    uint8_t Temp;
    if (Key_Num)
    {
        Temp = Key_Num;
        Key_Num = 0;
        return Temp;
    }
    return 0;
}

/**
  * 函    数：获取按键状态
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~1，返回0代表按键按开，返回1代表按键1按下
  */
static uint8_t Key_GetState(void)
{
    /* 判断按键1是否按下，是返回1，否则返回0 */
    if (GPIO_ReadInputDataBit(Key_GPIO_Port, Key_Pin) == 0)
    {
        return 1;
    }
    
    /* 判断按键2是否按下，是则返回2，否则返回0 */
    //重复上述 if判断程序
    
    return 0;
}

/**
  * 函    数：按键定时中断
  * 参    数：无
  * 返 回 值：无
  * 
  * 函数内static为局部静态变量，退出该函数后，值不会丢失
  * 
  * 倍频器：Count自增，每20ms进一次该if程序，将定时器定时周期设定为1ms（通用时间基准），20ms可进行按键消抖
  * 
  * 注意事项：定时中断函数void TIM2_IRQHandler(void)放在了main函数下，
  *                     若直接将按键代码写在中断函数中，不利于按键代码的独立封装，主程序也会显得臃肿
  *                     若将中断函数放在按键模块下，表示按键模块独占这个定时器，其他模块再想用就不方便
  *                     所以在按键模块下定义按键定时中断函数Key_Tick，然后在中断函数调用按键定时中断函数
  */
void Key_Tick(void)
{
    static uint8_t Count;
    static uint8_t CurrState, PrevState;
    
    Count ++;
    if (Count >= 20)
    {
        Count = 0;
        
        //用户代码
        PrevState = CurrState;    //上一次的按键状态
        CurrState = Key_GetState();    //本次的按键状态
        
        if (CurrState == 0 && PrevState != 0)    //捕捉到一次按键按下松手的瞬间
        {
            Key_Num = PrevState;    //获取上一次的按键状态（非零表示对应的哪个按键被按下）
        }
    }
}

/*********************************************************************************************************************************/

/**
  * 函    数：按键获取键码（阻塞式）
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~1，返回0代表按键按开，返回1代表按键1按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t KeyStop_GetNum(void)
{
    uint8_t KeyNum = 0;		//定义变量，默认键码值为0
    
    /* 判断按键1是否按下，是返回1，否则返回0 */
    if (GPIO_ReadInputDataBit(Key_GPIO_Port, Key_Pin) == 0)    /* 读取引脚输入寄存器的状态，如果为0，则代表按键按下 */
    {
        Delay_ms(20);    /* 延时消抖 */
        while (GPIO_ReadInputDataBit(Key_GPIO_Port, Key_Pin) == 0);    //等待按键松开
        Delay_ms(20);    /* 延时消抖 */
        KeyNum = 1;    /* 置键码为1 */
    }
    
    /* 判断按键2是否按下，是返回2，否则返回0 */
    //重复上述 if判断程序
    
    return KeyNum;    //返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
