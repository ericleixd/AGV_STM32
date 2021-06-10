#ifndef BATTERY_STATE_READ_H
#define BATTERY_STATE_READ_H



#include "stm32f10x.h"


#define BATTERY_STATE_USART               USART3
#define BATTERY_STATE_USART_RX_GPIO_PIN   GPIO_Pin_11
#define BATTERY_STATE_USART_RX_GPIO_PORT  GPIOB
#define BATTERY_STATE_USART_RX_GPIO_RCC   RCC_APB2Periph_GPIOB

#define BATTERY_STATE_USART_TX_GPIO_PIN   GPIO_Pin_10
#define BATTERY_STATE_USART_TX_GPIO_PORT  GPIOB
#define BATTERY_STATE_USART_TX_GPIO_RCC   RCC_APB2Periph_GPIOB


#define BATTERY_STATE_USART_RCC                RCC_APB1Periph_USART3   
#define BATTERY_STATE_USART_APBxCLKCmd         RCC_APB1PeriphClockCmd
#define BATTERY_STATE_USART_GPIO_APBxCLKCmd    RCC_APB1PeriphClockCmd

#define BATTERY_STATE_USART_IRQn         USART3_IRQn
#define BATTERY_STATE_USART_IRHandler    USART3_IRQHandler


//DMA∫Í∂®“Â
#define BATTERY_STATE_USART_RX_DMA_RCC     RCC_AHBPeriph_DMA1
#define BATTERY_STATE_USART_RX_DMA_CHANEL  DMA1_Channel3
#define BATTERY_STATE_USART_DR_Addr    USART3_BASE+0x04
#define buffer_size 20

void BATTERY_STATE_USART_Init(void);
void BATTERY_STATE_USART_Send_msg(u8 *buf);
void BATTERY_STATE_USART_Send_byte(u8 data);
void BATTERY_STATE_Rx_DMA_Config(u32 memoryAddr, u16 BufferSize);

extern u8 flag;
extern u8 battery_data[buffer_size];




#endif  
