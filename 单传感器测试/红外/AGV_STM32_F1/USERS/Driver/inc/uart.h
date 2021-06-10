#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include <stdio.h>
#define USARTx               USART1
#define USARTx_RX_GPIO_PIN   GPIO_Pin_10
#define USARTx_RX_GPIO_PORT  GPIOA
#define USARTx_RX_GPIO_RCC   RCC_APB2Periph_GPIOA

#define USARTx_TX_GPIO_PIN   GPIO_Pin_9
#define USARTx_TX_GPIO_PORT  GPIOA
#define USARTx_TX_GPIO_RCC   RCC_APB2Periph_GPIOA


#define USARTx_RCC           RCC_APB2Periph_USART1   
#define USARTx_APBxCLKCmd    RCC_APB2PeriphClockCmd
#define USARTx_GPIO_APBxCLKCmd    RCC_APB2PeriphClockCmd

#define USARTx_IRQn         USART1_IRQn
#define USARTx_IRHandler    USART1_IRQHandler

void UART_Init(void);

#endif
