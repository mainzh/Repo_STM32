#include "Timer.h"


/**
  * 函    数：定时中断初始化
  * 参    数：1 / Reso，分辨率，Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
  * 参    数：Freq，频率，单位Hz，范围 [SYSCLK_FREQ / 65536 / 65536， SYSCLK_FREQ]
  * 返 回 值：无
  */
void Timer_Init(uint32_t Reso, float Freq)
{
    uint16_t ARR = Reso - 1;    //自动重装载值，即计数周期，范围 [0, 65535]
    uint16_t PSC = 72000000 / Freq / Reso - 1;    //预分频系数，其中SYSCLK_FREQ_72MHz，范围 [0 , 65535]
        //其中 SYSCLK_FREQ / Freq / Reso范围 [1，65536]，推出 Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
    
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //开启TIM2的时钟
    
    /*内部时钟源配置*/
    TIM_InternalClockConfig(TIM2);    //选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
    
    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //定义结构体变量
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = ARR;    //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;    //预分频系数PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    //重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);    //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元	
    
    /*中断输出配置*/
    /* 清除定时器更新标志位
       TIM_TimeBaseInit函数末尾，手动产生了更新事件
       若不清除此标志位，则开启中断后，会立刻进入一次中断
       如果不介意此问题，则不清除此标志位也可*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //开启TIM2的更新中断
    
    /*NVIC中断分组配置
      此分组配置在整个工程中仅需调用一次，
      若有多个中断，可以把此代码放在main函数内，while循环之前，
      若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //配置NVIC为分组2
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;    //定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;    //选择配置NVIC的TIM2线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    //指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //指定NVIC线路的抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);    //将结构体变量交给NVIC_Init，配置NVIC外设
    
    /*TIM使能*/
    TIM_Cmd(TIM2, ENABLE);    //使能TIM2，定时器开始运行
}

/**
  * 函    数：TIM2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  *           用户程序结束后，中断标志位必须清除，否则中断将连续不断地触发，导致主程序卡死，使用TIM_ClearITPendingBit
  * 放在使用文件下，编写用户程序
  
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)    //判断是否是TIM2的更新事件触发的中断
    {
        // 用户程序
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    // 清除TIM2更新事件的中断标志位
    }
}
*/
