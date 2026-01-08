#include "Servo.h"


/**
  * 函    数：舵机初始化
  * 参    数：1 / Reso，分辨率，Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
  * 参    数：Freq，频率，单位Hz，范围 [SYSCLK_FREQ / 65536 / 65536 , SYSCLK_FREQ]
  * 返 回 值：无
  */
void Servo_Init(uint32_t Reso, float Freq)
{
    uint16_t ARR = Reso - 1;    //自动重装载值，即计数周期，范围 [0, 65535]
    uint16_t PSC = 72000000 / Freq / Reso - 1;    //预分频系数，其中SYSCLK_FREQ_72MHz，范围 [0 , 65535]
        //其中 SYSCLK_FREQ / Freq / Reso范围 [1，65536]，推出 Reso范围 [SYSCLK_FREQ / Freq / 65536， SYSCLK_FREQ / Freq]
    
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //开启TIM1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //开启GPIOA的时钟
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
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
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);    //将结构体变量交给TIM_OC1Init，配置TIM1的输出比较通道1
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);    //高级定时器专属--MOE主输出使能
    
    /*TIM使能*/
    TIM_Cmd(TIM1, ENABLE);    //使能TIM1，定时器开始运行
}

/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
  */
void Servo_SetAngle(float Angle)
{
    TIM_SetCompare1(TIM1, Angle / 180 * 2000 + 500);    //设置占空比Duty = CCR / (ARR + 1)
    //将角度线性变换，对应到舵机要求的占空比范围上
}
