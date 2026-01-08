#include "BDCMotor.h"

/**
  * 函    数：直流电机初始化
  * 参    数：1 / Reso，分辨率，Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
  * 参    数：Freq，频率，单位Hz，范围 [SYSCLK_FREQ / 65536 / 65536 , SYSCLK_FREQ]
  * 返 回 值：无
  */
void BDCMotor_Init(uint32_t Reso, float Freq)
{
    uint16_t ARR = Reso - 1;    //自动重装载值，即计数周期，范围 [0, 65535]
    uint16_t PSC = 72000000 / Freq / Reso - 1;    //预分频系数，其中SYSCLK_FREQ_72MHz，范围 [0 , 65535]
        //其中 SYSCLK_FREQ / Freq / Reso范围 [1，65536]，推出 Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
    
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //开启TIM1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启GPIOA的时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    //将PA4和PA5引脚初始化为推挽输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    //将PA8引脚初始化为复用推挽输出	
                                                                    //受外设控制的引脚，均需要配置为复用模式
    
    /*配置时钟源*/
    TIM_InternalClockConfig(TIM1);    //选择TIM1为内部时钟，若不调用此函数，TIM默认也为内部时钟
    
    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;    //定义结构体变量
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = ARR;    //自动重装载值ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;    //预分频系数PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    //重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);    //将结构体变量交给TIM_TimeBaseInit，配置TIM1的时基单元
    
    /*输出比较初始化*/ 
    TIM_OCInitTypeDef TIM_OCInitStructure;    //定义结构体变量
    TIM_OCStructInit(&TIM_OCInitStructure);    //结构体初始化，若结构体没有完整赋值
    //则最好执行此函数，给结构体所有成员都赋一个默认值
    //避免结构体初值不确定的问题
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    //输出比较模式，选择PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //输出极性，选择为高，若选择极性为低，则输出高低电平取反
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;    //初始的CCR值
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //将结构体变量交给TIM_OC3Init，配置TIM1的输出比较通道1
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);    //高级定时器专属--MOE主输出使能
    
    /*TIM使能*/
    TIM_Cmd(TIM1, ENABLE);    //使能TIM1，定时器开始运行
}

/**
  * 函    数：直流电机设置速度
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void BDCMotor_SetSpeed(int8_t Speed)
{
    if (Speed >= 0)    //如果设置正转的速度值
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);    //PA4置高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);    //PA5置低电平，设置方向为正转
        TIM_SetCompare1(TIM1, Speed);    //设置CCR1的值，占空比Duty = CCR / (ARR + 1)，PWM设置为速度值
    }
    else    //否则，即设置反转的速度值
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);    //PA4置低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_5);    //PA5置高电平，设置方向为反转
        TIM_SetCompare1(TIM1, -Speed);    //设置CCR1的值，占空比Duty = CCR / (ARR + 1)，
                                                             //PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
    }
}
