#ifndef __MOTOR_H
#define __MOTOR_H

#include "tim.h"
#include "foc_utils.h"

#include "AS5600.h"

/*内部使用*/
#define m_pwm_htimx         htim1          /* 电机定时器句柄*/
#define MOS_EN_GPIO_Port    GPIOB          /* MOS使能口 */
#define MOS_EN_Pin          GPIO_PIN_12    /* MOS使能引脚 */

#define voltage_power_supply    12    /* 供电电压 */
#define  pole_pairs              7    /* 电机磁极对数 */
#define Sensor_DIR              1    /* 传感器方向 */

/*设置三相的状态*/
#define PWM_Period (m_pwm_htimx.Init.Period + 1)

#define U0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_1 , 0);
#define V0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_2 , 0);
#define W0 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_3 , 0);
#define U1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_1 , PWM_Period/10);
#define V1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_2 , PWM_Period/10);
#define W1 __HAL_TIM_SET_COMPARE(&m_pwm_htimx , TIM_CHANNEL_3 , PWM_Period/10);

/*外部使用*/
void motor_init(void);    /* 电机初始化 */
void motor_run(void);    /* 电机运行 */
void motor_stop(void);    /* 电机停止 */

void motor_6step(float ms);    /* 电机六步相 */

void setPhaseVoltage(float Uq, float Ud, float angle_el);
void FOC_openLoop_velocity(float Uq, float rate_angle_el);    /* 验证setPhaseVoltage函数 */

void systick_CountMode(void);
float velocityOpenloop(float Uq ,float target_velocity);    /* 速度开环 */
float angleOpenloop(float Uq ,float velocity_limit ,float target_angle);    /* 角度开环 */

float _electricalAngle(void);    /* 计算电角度 */
void alignSensor(void);    /* 电角度校准 */

#endif    /* __MOTOR_H */
