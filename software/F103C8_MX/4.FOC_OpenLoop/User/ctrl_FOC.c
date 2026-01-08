#include "ctrl_FOC.h"


/* normalizing radian angle to [0,2PI] */
float _normalizeAngle(float angle){
  float a = fmod(angle, _2PI);
  return a >= 0 ? a : (a + _2PI);
}

/* FOC核心函数：输入Ud、Uq和电角度，输出PWM 采用SVPWM */
void setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    float Uref;
    float U_alpha, U_beta;
    float T0, T1, T2;
    float Ta, Tb, Tc;
    int sector;
    
    /* 归一化电角度值[0,2pi]，加90度后是参考电压矢量的位置 */
    if(Uq > 0)
    {
        angle_el = _normalizeAngle(angle_el + _PI_2);
    }
    else
    {
        angle_el = _normalizeAngle(angle_el - _PI_2);
    }
            
    /* park逆变换 */
    U_alpha = Ud * cos(angle_el) - Uq * sin(angle_el);
    U_beta = Ud * sin(angle_el) + Uq * cos(angle_el);
    
    /* 计算参考电压矢量的幅值 */
    Uref = _sqrt(U_alpha * U_alpha + U_beta * U_beta) / voltage_power_supply;
    /* 六边形的内切圆(SVPWM最大不失真旋转电压矢量赋值)根号3/3 */
    if(Uref > 0.577) Uref = 0.577;
    if(Uref < -0.577) Uref = -0.577;
    
    /* 判断参考电压矢量所在扇区 */
    sector = angle_el / _PI_3 + 1;
    
    /* 计算相邻电压矢量作用时间 */
    T1 = _SQRT3 * sin(sector * _PI_3 - angle_el) * Uref;
    T2 = _SQRT3 * sin(angle_el - (sector - 1.0) * _PI_3) * Uref;
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
