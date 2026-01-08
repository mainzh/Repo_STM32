#include "ex_motor.h"


/* 电机PWM输出开始 */
void PWM_start(void)
{
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_3);
    
    /* MOS使能 */
    HAL_GPIO_WritePin(MOS_EN_GPIO_Port, MOS_EN_Pin, GPIO_PIN_SET);
}

/* 6步换相变频驱动 */
/* ms    关断时间 毫秒
 * 不可长时间运行
 */
void ctrl_6step(float ms)
{
    /*U相通*/
    U1;V0;W0;
    HAL_Delay(ms);
    /*V相通*/
    U0;V1;W0;
    HAL_Delay(ms);
    /*W相通*/
    U0;V0;W1;
    HAL_Delay(ms);
}

/* ******************************************************************************** */
/* 配置PWM占空比 */
void setPWM(float Ua ,float Ub ,float Uc)
{
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_1, Ua);
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_2, Ub);
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_3, Uc);
}

/* 标定初始电角度 */
void adjust_electricangle(void)
{
    for(int i = 0; i < 500; i++)
    {
        setPhaseVoltage(0, 2, 0);
    }
    
//    zero_electric_angle = _electricalAngle();
    
//    setPhaseVoltage(3, 0, _3PI_2);
//    HAL_Delay(1000);
//    setPhaseVoltage(0, 0, _3PI_2);
}

/* 电流速度开环力矩控制 */
void FOC_openLoop_velocity(float Uq, float rate_angle_el)
{
    static float angle_el;
    /*估计电角度*/
    angle_el += rate_angle_el;
    
    setPhaseVoltage(Uq, 0, angle_el);
}

/* ******************************************************************** */
/* 0xFFFFFF到0循环计数 */
void systick_CountMode(void)
{
    SysTick->LOAD = 0xFFFFFF-1;      //set reload register
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; //Enable SysTick Timer
}

unsigned long open_loop_timestamp;
float shaft_angle;//!< current motor angle

/* 速度开环 */
/* 单位：rad/s */
float velocityOpenloop(float Uq ,float target_velocity)
{
    unsigned long now_us;
    float Ts;
    
    now_us = SysTick->VAL; //_micros();
    if(now_us<open_loop_timestamp)Ts = (float)(open_loop_timestamp - now_us)/9*1e-6;
    else
        Ts = (float)(0xFFFFFF - now_us + open_loop_timestamp)/9*1e-6;
    open_loop_timestamp=now_us;  //save timestamp for next call
    // quick fix for strange cases (micros overflow)
    if(Ts == 0 || Ts > 0.5) Ts = 1e-3; 
    
    // calculate the necessary angle to achieve target velocity
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity*Ts); 
    
    // set the maximal allowed voltage (voltage_limit) with the necessary angle
    setPhaseVoltage(Uq,  0, shaft_angle * m_pole_pairs);
    
    return Uq;
}

#define _sign(a) ( ( (a) < 0 )  ?  -1   : ( (a) > 0 ) )

/* 角度开环 */
float angleOpenloop(float Uq ,float velocity_limit ,float target_angle)
{
    unsigned long now_us;
    float Ts;
    
    now_us = SysTick->VAL; //_micros();
    if(now_us<open_loop_timestamp)Ts = (float)(open_loop_timestamp - now_us)/9*1e-6;
    else
        Ts = (float)(0xFFFFFF - now_us + open_loop_timestamp)/9*1e-6;
    open_loop_timestamp = now_us;  //save timestamp for next call
    // quick fix for strange cases (micros overflow)
    if(Ts == 0 || Ts > 0.5) Ts = 1e-3; 
    
    // calculate the necessary angle to move from current position towards target angle
    // with maximal velocity (velocity_limit)
    if(fabs( target_angle - shaft_angle ) > velocity_limit*Ts)
    {
    shaft_angle += _sign(target_angle - shaft_angle) * velocity_limit * Ts;
    //shaft_velocity = velocity_limit;
    }
    else
    {
    shaft_angle = target_angle;
    //shaft_velocity = 0;
    }
    
    // set the maximal allowed voltage (voltage_limit) with the necessary angle
    setPhaseVoltage(Uq,  0, shaft_angle * m_pole_pairs);
    
    return Uq;
}

/* ****************************************************************************************** */
