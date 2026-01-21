#include "uart_printf.h"

/* ==================== 非阻塞printf核心配置 ==================== */
#define PRINTF_BUF_SIZE 256  // 打印缓冲区大小（根据需求调整）
uint8_t printf_buf[PRINTF_BUF_SIZE];  // 环形缓冲区
uint16_t printf_buf_head = 0;         // 读指针（DMA发送用）
uint16_t printf_buf_tail = 0;         // 写指针（中断/主循环写用）
uint8_t uart_tx_busy = 0;             // 串口发送忙标志
uint8_t uart1_rx_temp = 0;            // 接收临时变量（补充定义）

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

//FILE __stdout; /* __FILE在stdio.h中定义 */

/* 环形缓冲区写入函数
   data      输入数据
   len       待写入长度
   返回值    实际写入长度
   使用场景  中断/非中断均可调用
 */
static uint16_t printf_buf_write(uint8_t *data, uint16_t len)
{
    uint16_t write_len = 0;
    // 关中断保护（避免读写指针冲突，中断中调用时也需关）
    __disable_irq();
    while (len > 0)
    {
        uint16_t next_tail = (printf_buf_tail + 1) % PRINTF_BUF_SIZE;
        if (next_tail == printf_buf_head) break;  // 缓冲区满，退出
        
        printf_buf[printf_buf_tail] = *data++;
        printf_buf_tail = next_tail;
        write_len++;
        len--;
    }
    __enable_irq();
    return write_len;
}

/* 缓冲区发送检查（核心：自动触发DMA发送，无需主循环调用） */
static void printf_buf_try_send(void)
{
    if (uart_tx_busy) return;  // 发送中，跳过
    
    // 计算待发送长度（关中断保护指针）
    uint16_t len = 0;
    __disable_irq();
    if (printf_buf_head != printf_buf_tail)
    {
        len = (printf_buf_tail - printf_buf_head + PRINTF_BUF_SIZE) % PRINTF_BUF_SIZE;
        // 限制单次发送长度（避免DMA传输过长，适配F4的DMA最大传输长度）
        if (len > 64) len = 64;
    }
    __enable_irq();
    
    // 开启DMA发送
    if (len > 0)
    {
        uart_tx_busy = 1;
        
        #if RS485_EN
            RS485_TX;
        #endif
        // 检查DMA状态，避免重复启动（增强鲁棒性）
        if (HAL_UART_Transmit_DMA(&UartHandle, &printf_buf[printf_buf_head], len) == HAL_OK)
        {
            // 更新读指针（仅在DMA启动成功后更新）
            __disable_irq();
            printf_buf_head = (printf_buf_head + len) % PRINTF_BUF_SIZE;
            __enable_irq();
        }
        else
        {
            uart_tx_busy = 0;  // DMA启动失败，重置忙标志
        }
    }
}

/* 重定向printf（替换默认的fputc） */
PUTCHAR_PROTOTYPE
{
    #pragma diag_suppress 870 /* 屏蔽870警告，多由文件编码问题导致，如代码包含中文字符 */

    // 仅处理串口输出（忽略其他流）
    if (f == stdout || f == stderr)
    {
        // 中断中调用时，仅写入缓冲区；非中断时，写入后自动尝试发送
        uint32_t is_in_irq = __get_PRIMASK();  // 判断是否在中断上下文（PRIMASK=1表示关中断/在中断）
        printf_buf_write((uint8_t*)&ch, 1);
        
        // 非中断上下文：立即尝试发送（避免数据堆积）
        if (is_in_irq == 0)
        {
            printf_buf_try_send();
        }
    }
    return ch;
}

/* 串口DMA发送完成回调：自动触发下一次发送 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USARTx)
    {
        // 判断是否为DMA模式：hdmatx不为NULL 且 DMA通道未就绪
        if (huart->hdmatx != NULL && huart->hdmatx->State != HAL_DMA_STATE_READY)/* USARTx DMA发送完成 */
        {
            uart_tx_busy = 0;  // 发送完成，清除忙标志
            
            #if RS485_EN
                RS485_RX;  // 发送完成后切换回接收模式
            #endif
            
            printf_buf_try_send();  // 自动检查并发送下一批数据
        }
        else    /* USARTx 中断发送完成 */
        {
            
        }
    }
}
