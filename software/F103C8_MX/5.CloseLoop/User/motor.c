#include "motor.h"


/* 电机初始化 */
void motor_init(void)
{
    /* 定时器PWM输出开始 */
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&m_pwm_htimx , TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&m_pwm_htimx , TIM_CHANNEL_3);
}

/* 电机运行 */
void motor_run(void)
{
    HAL_GPIO_WritePin(MOS_EN_GPIO_Port, MOS_EN_Pin, GPIO_PIN_SET);
}

/* 电机停止 */
void motor_stop(void)
{
    HAL_GPIO_WritePin(MOS_EN_GPIO_Port, MOS_EN_Pin, GPIO_PIN_RESET);
}

/* ***************************************************************** */
/* 电机六步相
 * 直流无刷电机换相变频驱动 不可长时间运行
 * ms    关断时间 毫秒
 */
void motor_6step(float ms)
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

/* ***************************************************************** */
/* 配置PWM占空比 */
void setPWM(float Ua ,float Ub ,float Uc)
{
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_1, Ua);
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_2, Ub);
    __HAL_TIM_SET_COMPARE(&m_pwm_htimx, TIM_CHANNEL_3, Uc);
}

/* FOC核心函数：输入Ud、Uq和电角度，输出PWM 采用SVPWM */
void setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    float Uout;
    uint32_t sector;
    float T0,T1,T2;
    float Ta,Tb,Tc;
    
    if(Ud) // only if Ud and Uq set 
    {// _sqrt is an approx of sqrt (3-4% error)
        Uout = _sqrt(Ud*Ud + Uq*Uq) / voltage_power_supply;
        // angle normalisation in between 0 and 2pi
        // only necessary if using _sin and _cos - approximation functions
        angle_el = _normalizeAngle(angle_el + atan2(Uq, Ud));
    }
    else
    {// only Uq available - no need for atan2 and sqrt
        Uout = Uq / voltage_power_supply;
        // angle normalisation in between 0 and 2pi
        // only necessary if using _sin and _cos - approximation functions
        angle_el = _normalizeAngle(angle_el + _PI_2);
    }
    
    /* 六边形的内切圆(SVPWM最大不失真旋转电压矢量赋值)根号3/3 */
    if(Uout > 0.577f) Uout = 0.577f;
    if(Uout < -0.577) Uout = -0.577f;
    
    /* 判断参考电压矢量所在扇区 */
    sector = (angle_el / _PI_3) + 1;
    
    /* 计算相邻电压矢量作用时间 */
    T1 = _SQRT3*_sin(sector*_PI_3 - angle_el) * Uout;
    T2 = _SQRT3*_sin(angle_el - (sector-1.0)*_PI_3) * Uout;
    /* 计算零矢量作用时间 */
    T0 = 1 - T1 - T2;
    
    /* 计算各相桥臂的开关时间 */
    switch(sector)
    {
        case 1:
            Ta = T1 + T2 + T0 / 2;
            Tb = T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 2:
            Ta = T1 +  T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 3:
            Ta = T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T2 + T0 / 2;
            break;
        case 4:
            Ta = T0 / 2;
            Tb = T1+ T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 5:
            Ta = T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 6:
            Ta = T1 + T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T0 / 2;
            break;
        /*possible error state*/
        default:
            Ta = 0;
            Tb = 0;
            Tc = 0;
    }
    
    /* 配置PWM占空比 */
    setPWM(Ta * PWM_Period, Tb * PWM_Period, Tc * PWM_Period);
}

/* 验证setPhaseVoltage函数 */
void FOC_openLoop_velocity(float Uq, float rate_angle_el)
{
    static float angle_el;
    /*估计电角度*/
    angle_el += rate_angle_el;
    
    setPhaseVoltage(Uq, 0, angle_el);
}

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
    setPhaseVoltage(Uq,  0, shaft_angle * pole_pairs);
    
    return Uq;
}

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
    setPhaseVoltage(Uq,  0, shaft_angle * pole_pairs);
    
    return Uq;
}

/* ***************************************************************** */

float zero_electric_angle;    /* 初始电角度 */

/* 计算电角度 */
float _electricalAngle(void)
{
  return  _normalizeAngle((float)(Sensor_DIR *  pole_pairs) * as5600GetAngle() - zero_electric_angle);
}

/* 电角度校准 */
void alignSensor(void)
{
    setPhaseVoltage(3, 0, _3PI_2);
    HAL_Delay(3000);
    
    zero_electric_angle = _electricalAngle();
    
    setPhaseVoltage(0, 0, _3PI_2);
}

/* ***************************************************************** */
