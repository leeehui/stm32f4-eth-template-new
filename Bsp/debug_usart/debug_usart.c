
  
#include "debug_usart.h"



///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		HAL_UART_Transmit(&DEBUG_USART, (uint8_t *) &ch, 1, 0xFFFF);
		return (ch);
}

/*********************************************END OF FILE**********************/
