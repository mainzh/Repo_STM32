#ifndef __EX_MOTOR_H
#define __EX_MOTOR_H

#include "tim.h"
#include "ctrl_FOC.h"


/*内部使用*/
#define m_pwm_htimx htim1    /* 电机定时器句柄*/
#define MOS_EN_GPIO_Port GPIOB    /* MOS使能口 */
#define MOS_EN_Pin GPIO_PIN_12    /* MOS使能引脚 */

#define voltage_power_supply 12    /* 电机供电电压*/
#define m_pole_pairs 7    /* 电机磁极对数 */

/*设置三相的状态*/
#define PWM_Period (m_pwm_htimx.Init.Period + 1)

#define U0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_1 , 0);
#define V0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_2 , 0);
#define W0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_3 , 0);
#define U1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_1 , PWM_Period/10);
#define V1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_2 , PWM_Period/10);
#define W1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_3 , PWM_Period/10);

/*外部使用*/
void PWM_start(void);    /* 电机PWM输出开始 */

void ctrl_6step(float ms);    /* 6步换相变频驱动 */

void setPWM(float Ua ,float Ub ,float Uc);    /* 配置PWM占空比 */

void adjust_electricangle(void);    /* 标定初始电角度 */

void FOC_openLoop_velocity(float Uq, float rate_angle_el);    /* 电流速度开环力矩控制 */

void systick_CountMode(void);

float velocityOpenloop(float Uq ,float target_velocity);

float angleOpenloop(float Uq ,float velocity_limit ,float target_angle);

#endif    /* __EX_MOTOR_H */
