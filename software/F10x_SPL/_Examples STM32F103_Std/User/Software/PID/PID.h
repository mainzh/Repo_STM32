#ifndef __PID_H
#define __PID_H


#include "stm32f10x.h"                  // Device header

/* PID相关参数 */

#define  INCR_LOCT_SELECT  1         /* 0：位置式，1：增量式 */

#if INCR_LOCT_SELECT         /* 增量式 */

/* 定义位置环PID参数相关宏 */
#define  L_KP      0.0f             /* P参数 */
#define  L_KI      0.00f             /* I参数 */
#define  L_KD      0.0f             /* D参数 */

/* 定义速度环PID参数相关宏 */
#define  S_KP      0.05f             /* P参数 */
#define  S_KI      0.001f             /* I参数 */
#define  S_KD      0.00f             /* D参数 */

/* 定义电流环PID参数相关宏 */
#define  C_KP      0.00f             /* P参数 */
#define  C_KI      0.00f             /* I参数 */
#define  C_KD      0.00f             /* D参数 */
#define  SMAPLSE_PID_SPEED  50       /* 采样周期 单位ms */

#else         /* 位置式 */

/* 定义位置环PID参数相关宏 */
#define  L_KP      0.00f             /* P参数 */
#define  L_KI      0.00f             /* I参数 */
#define  L_KD      0.00f             /* D参数 */

/* 定义速度环PID参数相关宏 */
#define  S_KP      0.05f             /* P参数 */
#define  S_KI      0.001f             /* I参数 */
#define  S_KD      0.00f             /* D参数 */

/* 定义电流环PID参数相关宏 */
#define  C_KP      0.00f             /* P参数 */
#define  C_KI      0.00f             /* I参数 */
#define  C_KD      0.00f             /* D参数 */
#define  SMAPLSE_PID_SPEED  50       /* 采样周期 单位ms */

#endif


/*PID结构体*/
typedef struct
{
    float  SetPoint;            /* 目标值 */
    float  ActualValue;         /* 期望输出值 */
    float  SumError;            /* 误差累计 */
    float  Proportion;          /* 比例常数 P */
    float  Integral;            /* 积分常数 I */
    float  Derivative;          /* 微分常数 D */
    float  Error;               /* Error[1] */
    float  LastError;           /* Error[-1] */
    float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_location_pid;  /* 位置环PID参数结构体 */
extern PID_TypeDef  g_speed_pid;     /* 速度环PID参数结构体 */
extern PID_TypeDef  g_current_pid;   /* 电流环PID参数结构体 */


void pid_init(void);                 /* pid初始化 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value);              /* pid闭环控制 */
void integral_limit( PID_TypeDef *PID , float max_limit, float min_limit );     /* 积分限幅 */

#endif
