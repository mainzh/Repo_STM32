/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_DEBUGGER_H__
#define __UART_DEBUGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include <stdio.h>

/* 使用RS485模块 */

#define RS485_EN 1

#if RS485_EN
  #define RS485_EN_GPIO_Port GPIOA            /* RS485发送使能引脚 */
  #define RS485_EN_Pin       GPIO_PIN_8
  
  #define RS485_TX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);    /* 置1为串口发送 */
  #define RS485_RX HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);  /* 置0为串口接收 */
#endif

#define UartHandle    huart1         /* 串口所使用的句柄 */
#define USARTx        USART1         /* 使用第几号串口 */
#define DMAUartHandle hdma_usart1_rx /* 串口DMA接收通道的句柄 */

extern DMA_HandleTypeDef DMAUartHandle;

/* Size of Reception buffer */
#define RXBUFFERSIZE 256

extern uint8_t aRxBuffer[];

void UART_Receive_Start(void);

void UART_Receive_IT_Start(void);
void UART_Receive_DMA_Start(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_DEBUGGER_H__ */
