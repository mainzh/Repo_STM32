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
