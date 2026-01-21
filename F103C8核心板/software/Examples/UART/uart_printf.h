#ifndef __UART_PRINTF_H__
#define __UART_PRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h"
#include <stdio.h>


/* 使用RS485模块 */

#define RS485_EN 1

#if RS485_EN
  #define RS485_EN_GPIO_Port GPIOA            /* RS485发送使能引脚 */
  #define RS485_EN_Pin       GPIO_PIN_8
  
  #define RS485_TX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET)    /* 置1为串口发送 */
  #define RS485_RX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET)  /* 置0为串口接收 */
#endif

#define UartHandle    huart1         /* 串口所使用的句柄 */
#define USARTx        USART1         /* 使用第几号串口 */

#ifdef __cplusplus
}
#endif

#endif /* __UART_PRINTF_H__ */
