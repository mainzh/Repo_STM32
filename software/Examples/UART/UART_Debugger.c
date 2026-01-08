/* Includes ------------------------------------------------------------------*/
#include "UART_Debugger.h"

/* 串口收发-轮询模式 ---------------------------------------------------------------*/

/** UART_Printf_init function
    
    STM32CubeMX 配置串口
    1. Put the USART peripheral in the Asynchronous (异步) mode (UART Mode)
    2. Hardware Flow Control 硬件流控制选择：Disable
    3. Baud Rate 波特率：115200 Bits/s
    4. Word Length 字长：8 Bits (including Parity)
    5. Parity 奇偶校验位：None
    6. Stop Bits 停止位：1
    7. Data Direction 数据方向：Receive and Transmit (printf只需打开Transmit即可，这里为了方便都打开了)
    8. Over Sampling 过采样：16 Samples
    
    MX_USART1_UART_Init();
    
    HAL库 串口GPIO配置
    1. GPIO_InitTypeDef GPIO_InitStruct = {0}; 初始化UART1的GPIO结构体
    2. if(uartHandle->Instance==USART1) 判断串口实例是否为USART1
    3. __HAL_RCC_USART1_CLK_ENABLE(); 开启UART1时钟
    4. __HAL_RCC_GPIOA_CLK_ENABLE(); 开启UART1的GPIO时钟
    5. GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; 配置USART1_TX、USART1_RX的Pin脚
    6. GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 配置GPIO复用推挽输出模式
    7. GPIO_InitStruct.Pull = GPIO_NOPULL; 配置GPIO浮空(无上拉、无下拉)
    8. GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 配置GPIO速率为非常高
    9. GPIO_InitStruct.Alternate = GPIO_AF7_USART1; 配置GPIO复用为USART1功能
    10. HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 将以上参数传入HAL_GPIO_Init函数，进行GPIO初始化
    
    HAL_UART_MspInit();
*/

/* Use "printf" function without definition "use MicroLIB" */

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/**不使用半主机模式，至少需重定义 __ttywrch函数、_sys_exit函数、_sys_command_string函数，
   以同时兼容AC5和AC6模式
*/
#if (__ARMCC_VERSION >= 6010050) /* 使用AC6编译器 */
__asm(".global __use_no_semihosting\n\t");    /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* 声明main函数为无参数格式，否则部分例程可能出现半主机模式 */
#else /* 使用AC5编译器 */
#pragma import(__use_no_semihosting) /* 声明不使用半主机模式 */

struct __FILE /* 定义__FILE */
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
#endif

int _ttywrch(int ch) /* 重定义 __ttywrch函数 */
{
    ch = ch;
    return ch;
}

void _sys_exit(int x) /* 重定义 _sys_exit函数 */
{
    x = x;
}

char *_sys_command_string(char *cmd, int len) /* 重定义 _sys_command_string函数 */
{
    return NULL;
}

FILE __stdout; /* __FILE在stdio.h中定义 */

PUTCHAR_PROTOTYPE /* 重定义fputc函数，供printf函数最终调用，输出字符串 */
{
    #pragma diag_suppress 870 /* 屏蔽870警告，多由文件编码问题导致，如代码包含中文字符 */
    
#if RS485_EN
    RS485_TX
#endif
    
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    
    return ch;
}

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

/* 串口收发-轮询模式测试 */
void UART_Receive_Start(void){
    printf("串口收发-轮询模式    \n\r");
    
    /* UART_Printf测试 */
    printf("→上位机：UART_Printf测试    1a.#。\n\r");
    
    /* UART_Receive测试 */
#if RS485_EN
    RS485_RX
#endif
    HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, HAL_MAX_DELAY);
    printf("→MCU：串口接收    ");
    
    /* UART_Transmit测试 */
#if RS485_EN
    RS485_TX
#endif
    HAL_UART_Transmit(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, HAL_MAX_DELAY);
    printf("\n\r");
    
    /* 数据处理 */
    
}

/* 串口收发-中断模式 -----------------------------------------------------------------*/

/* UART_Receive_IT init function */

/** UART_Receive_IT_init
    
    STM32CubeMX 配置串口
    1. Put the USART peripheral in the Asynchronous (异步) mode (UART Mode)
    2. Hardware Flow Control 硬件流控制选择：Disable
    3. Baud Rate 波特率：115200 Bits/s
    4. Word Length 字长：8 Bits (including Parity)
    5. Parity 奇偶校验位：None
    6. Stop Bits 停止位：1
    7. Data Direction 数据方向：Receive and Transmit (printf只需打开Transmit即可，这里为了方便都打开了)
    8. Over Sampling 过采样：16 Samples
    
    MX_USART1_UART_Init();
    
    HAL库 串口GPIO配置
    1. GPIO_InitTypeDef GPIO_InitStruct = {0}; 初始化UART1的GPIO结构体
    2. if(uartHandle->Instance==USART1) 判断串口实例是否为USART1
    3. __HAL_RCC_USART1_CLK_ENABLE(); 开启UART1时钟
    4. __HAL_RCC_GPIOA_CLK_ENABLE(); 开启UART1的GPIO时钟
    5. GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; 配置USART1_TX、USART1_RX的Pin脚
    6. GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 配置GPIO复用推挽输出模式
    7. GPIO_InitStruct.Pull = GPIO_NOPULL; 配置GPIO浮空(无上拉、无下拉)
    8. GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 配置GPIO速率为非常高
    9. GPIO_InitStruct.Alternate = GPIO_AF7_USART1; 配置GPIO复用为USART1功能
    10. HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 将以上参数传入HAL_GPIO_Init函数，进行GPIO初始化
    11. HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); 配置USART1的中断优先级
    12. HAL_NVIC_EnableIRQ(USART1_IRQn); 使能USART1的中断
    
    HAL_UART_MspInit
*/
void UART_Receive_IT_Start(void)
{
    /* Start the transmission process #####################################*/  
    /* While the UART in reception process, user can transmit data through 
       "aTxBuffer" buffer */
//#if RS485_EN
//    RS485_TX
//#endif
//    
//    if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
//    {
//        /* Transfer error in transmission process */
//        while(1)
//        {
//        }
//    }
    
    /* Put UART peripheral in reception process ###########################*/  
    /* Any data received will be stored "aRxBuffer" buffer : the number max of 
       data received is RXBUFFERSIZE */
#if RS485_EN
    RS485_RX
#endif
    
    if(HAL_UART_Receive_IT(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
        /* Transfer error in reception process */
        while(1)
        {
        }
    }
    
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Transfer in reception process is correct */
    
    /* 判断使用此回调函数的串口 */
    if(huart->Instance==USARTx)
    {
        /* 处理接收的数据 */
        /* 1.发送接收到的数据 */
        #if RS485_EN
            RS485_TX
        #endif
        HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)aRxBuffer, sizeof(aRxBuffer));
//        HAL_UART_Transmit(&UartHandle, (uint8_t*)aRxBuffer, RXBUFFERSIZE,HAL_MAX_DELAY);
        
        /* 2.数据的处理逻辑 */
        
        
        /* 再次开启串口接收中断 */
        UART_Receive_IT_Start();
    }
}

/* 串口收发-DMA模式 -----------------------------------------------------------------*/

/* UART_Receive_DMA init function */

/** UART_Receive_DMA_init
    
    STM32CubeMX 配置串口
    1. Put the USART peripheral in the Asynchronous (异步) mode (UART Mode)
    2. Hardware Flow Control 硬件流控制选择：Disable
    3. Baud Rate 波特率：115200 Bits/s
    4. Word Length 字长：8 Bits (including Parity)
    5. Parity 奇偶校验位：None
    6. Stop Bits 停止位：1
    7. Data Direction 数据方向：Receive and Transmit (printf只需打开Transmit即可，这里为了方便都打开了)
    8. Over Sampling 过采样：16 Samples
    
    MX_USART1_UART_Init();
    
    HAL库 串口GPIO配置
    1. GPIO_InitTypeDef GPIO_InitStruct = {0}; 初始化UART1的GPIO结构体
    2. if(uartHandle->Instance==USART1) 判断串口实例是否为USART1
    3. __HAL_RCC_USART1_CLK_ENABLE(); 开启UART1时钟
    4. __HAL_RCC_GPIOA_CLK_ENABLE(); 开启UART1的GPIO时钟
    5. GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10; 配置USART1_TX、USART1_RX的Pin脚
    6. GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; 配置GPIO复用推挽输出模式
    7. GPIO_InitStruct.Pull = GPIO_NOPULL; 配置GPIO浮空(无上拉、无下拉)
    8. GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 配置GPIO速率为非常高
    9. GPIO_InitStruct.Alternate = GPIO_AF7_USART1; 配置GPIO复用为USART1功能
    10. HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 将以上参数传入HAL_GPIO_Init函数，进行GPIO初始化
    11. USART1 DMA Init 
    12. HAL_NVIC_SetPriority(USART1_IRQn, 0, 0); 配置USART1的中断优先级
    13. HAL_NVIC_EnableIRQ(USART1_IRQn); 使能USART1的中断
    
    HAL_UART_MspInit
    
    串口收发-DMA模式下，aRxBuffer的RXBUFFERSIZE应设置的大一点，256
*/
void UART_Receive_DMA_Start(void)
{
    #if RS485_EN
        RS485_RX
    #endif
    /* 关闭DMA传输过半中断(否则也将触发RxEventCallback回调函数) */
    __HAL_DMA_DISABLE_IT(&DMAUartHandle, DMA_IT_HT);    /* (DMA通道的指针地址 , 要关闭的中断) */
    
    /* 开启串口DMA接收空闲中断 */
    HAL_UARTEx_ReceiveToIdle_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE);
    
    __HAL_UART_CLEAR_IDLEFLAG(&UartHandle);    /* 清除Idle中断标志 (避免首次触发异常) */
}

/* UART接收事件回调函数 */
/* Size 是HAL库返回的Idle时已接收的字节数 (实际接收的数据长度) */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    /* 判断使用此回调函数的串口 */
    if(huart->Instance==USARTx)
    {
        /* 处理接收的数据 */
        if(Size > 0){    /* 当实际接收的数据长度 > 0 */
            /* 1.发送接收到的数据 */
            #if RS485_EN
                RS485_TX
            #endif
            HAL_UART_Transmit_DMA(&UartHandle, (uint8_t *)aRxBuffer, Size);
//            HAL_UART_Transmit(&UartHandle, (uint8_t*)aRxBuffer, Size, HAL_MAX_DELAY);
            
            /* 2.数据的处理逻辑 */
            
        }
        
        /* 再次开启串口DMA接收空闲中断 */
        UART_Receive_DMA_Start();
    }
}

/* 串口发送完成回调函数 -----------------------------------------------------------------*/
/* 轮询模式：无发送完成回调
   轮询模式的发送函数 HAL_UART_Transmit() 是阻塞式的：函数开始执行后，CPU 会一直等待发送完成，
   直到所有字节发送完毕或超时，函数返回即表示发送完成，因此无需、也不会触发任何发送完成回调。
   // 轮询发送：函数返回即完成，无回调
   uint8_t tx_data[] = "Polling Transmit\r\n";
   HAL_UART_Transmit(&huart1, tx_data, sizeof(tx_data)-1, 100); // 超时100ms
   // 函数执行完后，直接处理发送完成逻辑
   printf("Polling Transmit Done\r\n");

   中断模式：触发通用完成回调 HAL_UART_TxCpltCallback
   中断发送函数 HAL_UART_Transmit_IT() 是非阻塞的：调用后 CPU 立即返回，发送过程由硬件中断完成，
   所有字节发送完毕后触发 HAL_UART_TxCpltCallback（与 DMA 模式共用）。

   DMA 模式：触发通用完成回调 + 可选专用半完成回调
   完整发送完成：所有字节发送完毕后，触发 HAL_UART_TxCpltCallback（与中断模式共用）；
   // 通用发送完成回调（中断/DMA共用）
   void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
   {
       if (huart->Instance == USART1)
       {
           // 发送完成逻辑（不管是中断还是DMA触发，逻辑通用）
           uart1_tx_busy = 0; // 清除发送忙标志
           printf("USART1 Transmit Done (IT/DMA)\r\n");
           
           // 可选：触发下一次发送
           // UART1_Start_Transmit_IT(next_tx_data, next_len);
       }
   }
 */
/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Transfer in transmission process is correct */
    if (huart->Instance == USART1)
    {
        // 判断是否为DMA模式：hdmatx不为NULL 且 DMA通道未就绪
        if (huart->hdmatx != NULL && huart->hdmatx->State != HAL_DMA_STATE_READY)
        {
            printf("USART1 DMA发送完成\r\n");
        }
        else
        {
            printf("USART1 中断发送完成\r\n");
        }
        
//        uart1_tx_busy = 0; // 清除发送忙标志
    }
}

/* DMA专用：发送半完成回调（仅DMA模式触发） */
/* 传输过半（可选）：若开启 DMA 的「传输过半（HT）中断」，发送到总长度一半时，
   会触发DMA 专用的半完成回调 HAL_UART_TxHalfCpltCallback（中断模式无此回调）。
 */
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USARTx)
    {
        // 仅DMA模式下，发送到一半时触发（如256字节发送到128字节）
        printf("USART1 Transmit Half Done (DMA Only)\r\n");
    }
}

/* 串口收发错误处理回调函数 -----------------------------------------------------------------*/
/* 不管是轮询、中断还是 DMA 模式，只要 UART 硬件检测到错误（如帧错误 FE、溢出错误 ORE、奇偶校验错误 PE 等），
   都会触发同一个 HAL_UART_ErrorCallback 函数。

   轮询模式：调用 HAL_UART_Transmit()/HAL_UART_Receive() 时，若检测到错误，函数会返回错误码（如HAL_ERROR），同时触发该回调；

   中断 / DMA 模式：错误发生时会立即触发该回调，无需等待函数返回。
 */
/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Transfer error in reception/transmission process */
    if (huart->Instance == USARTx)  /* 先区分串口实例*/
    {
        /* 1. 获取错误类型（适配所有模式） */
        uint32_t error_flag = HAL_UART_GetError(huart);
        
        /* 2. 针对不同错误类型处理 */
        if (error_flag & HAL_UART_ERROR_ORE)  // 溢出错误（常见）
        {
            __HAL_UART_CLEAR_OREFLAG(huart);  /* 清除错误标志 */
            /* 中断/DMA模式需重启收发，轮询模式需重新调用收发函数 */
            
        }
        else if (error_flag & HAL_UART_ERROR_FE)  /* 帧错误 */
        {
            __HAL_UART_CLEAR_FEFLAG(huart);
        }
        
        /* 3. 错误日志/报警（可选） */
        printf("USART1 Error: 0x%08X\r\n", error_flag);
    }
}
