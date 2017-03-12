#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


extern UART_HandleTypeDef huart1;

#define DEBUG_USART huart1


int fputc(int ch, FILE *f);

#endif /* __USART1_H */
