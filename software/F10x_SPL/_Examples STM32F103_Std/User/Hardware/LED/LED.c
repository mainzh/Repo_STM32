#include "LED.h"


/*引脚配置*/
#define LED_Pin GPIO_Pin_13
#define LED_GPIO_Port GPIOC


uint8_t LED_Mode;    //指定LED的模式
//若要控制多个LED灯，定义相应的LEDx_Mode，来控制相应的LEDx
uint16_t LED_Count;    //计次变量，LED的比较值
//若要控制多个LED灯，定义相应的LEDx_Count，来控制相应的LEDx


/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
    /*开启时钟
      使用每个外设前必须开启其时钟，否则对外设的操作无效 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_Pin;  /* GPIO引脚 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  /* GPIO翻转速率 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* GPIO模式 */
    GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);
    
    /*设置GPIO初始化后的默认电平*/
    GPIO_SetBits(LED_GPIO_Port, LED_Pin);
}

/**************************************************************************************/

/**
  * 函    数：LED开启
  * 参    数：无  （根据硬件）
  * 返 回 值：无
  */
void LED_ON(void)
{
    GPIO_ResetBits(LED_GPIO_Port, LED_Pin);    /* 设置引脚为低电平 */
//    GPIO_WriteBit(LED_GPIO_Port, LED_Pin, Bit_RESET);    /* 设置引脚为低电平 */
}

/**
  * 函    数：LED关闭
  * 参    数：无  （根据硬件）
  * 返 回 值：无
  */
void LED_OFF(void)
{
    GPIO_SetBits(LED_GPIO_Port, LED_Pin);    /* 设置引脚为高电平 */
//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);    /* 设置引脚为高电平 */
}

/**
  * 函    数：LED状态翻转
  * 参    数：无
  * 返 回 值：无
  */
void LED_Turn(void)
{
    if (GPIO_ReadOutputDataBit(LED_GPIO_Port, LED_Pin) == 0)    /* 读取GPIO输出寄存器的状态，如果当前引脚输出低电平 */
    {
        GPIO_SetBits(LED_GPIO_Port, LED_Pin);    /* 则设置引脚为高电平 */
    }
    else    /* 否则，即当前引脚输出高电平 */
    {
        GPIO_ResetBits(LED_GPIO_Port, LED_Pin);    /* 则设置引脚为低电平 */
    }
}

/**
  * 函    数：设置LED的模式
  * 参    数：Mode，0——LED关闭；1——LED打开；2——LED慢闪；3——LED快闪；4——LED点闪
  * 返 回 值：无
  * 注意事项：若正在执行定时中断函数LED_Tick中LED_Mode == 2， LED_Count自增到200，此时LED亮，
  *          切换为LED_Mode == 3，此时LED_Count将从200开始自增
  *          因此在设置LED的模式函数LED_SetMode中，使用如下if程序
  */
void LED_SetMode(uint8_t Mode)
{
    if (Mode != LED_Mode)
    {
        LED_Mode = Mode;
        LED_Count = 0;
    }
}

/**
  * 函    数：LED定时中断
  * 参    数：无
  * 返 回 值：无
  * 
  * 函数内static为局部静态变量，退出该函数后，值不会丢失
  * 
  * 倍频器：LED_Count自增，先设定周期，再设置LED亮的周期，将定时器定时周期设定为1ms（通用时间基准）
  * 
  * 注意事项：定时中断函数void TIM2_IRQHandler(void)放在了main函数下，
  *                     若直接将按键代码写在中断函数中，不利于按键代码的独立封装，主程序也会显得臃肿
  *                     若将中断函数放在按键模块下，表示按键模块独占这个定时器，其他模块再想用就不方便
  *                     所以在按键模块下定义按键定时中断函数Key_Tick，然后在中断函数调用按键定时中断函数
  */
void LED_Tick(void)
{
    //一个LED的控制程序
    if(LED_Mode == 0)    //LED关闭
    {
        LED_OFF();
    }
    else if(LED_Mode == 1)    //LED打开
    {
        LED_ON();
    }
    else if(LED_Mode == 2)    //LED亮500ms，灭500ms（慢闪）
    {
        LED_Count ++;
        
        //LED周期
//        if(LED_Count > 999) LED_Count = 0;    //LED_Count计数到999后，下一次计数LED_Count = 0  防止自增越界，也可使用下一行代码
        LED_Count %= 1000;    //LED_Count < 1000时，取余 = 本身；LED_Count = 1000时，取余 = 0；LED_Count > 1000时，取余 = 1000内的余数
        
        //LED比较值
        if(LED_Count < 500)
        {
            LED_ON();
        }
        else
        {
            LED_OFF();
        }
    }
    else if(LED_Mode == 3)    //LED亮50ms，灭50ms（快闪）
    {
        LED_Count ++;
        
        LED_Count %= 100;    //LED_Count < 1000时，取余 = 本身；LED_Count = 1000时，取余 = 0；LED_Count > 1000时，取余 = 1000内的余数
        
        if(LED_Count < 50)
        {
            LED_ON();
        }
        else
        {
            LED_OFF();
        }
    }
    else if(LED_Mode == 4)    //LED亮100ms，灭900ms（点闪）
    {
        LED_Count ++;
        
        LED_Count %= 1000;    //LED_Count < 1000时，取余 = 本身；LED_Count = 1000时，取余 = 0；LED_Count > 1000时，取余 = 1000内的余数
        
        if(LED_Count < 100)
        {
            LED_ON();
        }
        else
        {
            LED_OFF();
        }
    }
}
