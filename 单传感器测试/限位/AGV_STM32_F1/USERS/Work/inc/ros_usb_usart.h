#ifndef ROS_USB_USART_H
#define ROS_USB_USART_H

#include "stm32f10x.h"

#define ROS_USB_USART               USART1
#define ROS_USB_USART_RX_GPIO_PIN   GPIO_Pin_10
#define ROS_USB_USART_RX_GPIO_PORT  GPIOA
#define ROS_USB_USART_RX_GPIO_RCC   RCC_APB2Periph_GPIOA

#define ROS_USB_USART_TX_GPIO_PIN   GPIO_Pin_9
#define ROS_USB_USART_TX_GPIO_PORT  GPIOA
#define ROS_USB_USART_TX_GPIO_RCC   RCC_APB2Periph_GPIOA


#define ROS_USB_USART_RCC           RCC_APB2Periph_USART1   
#define ROS_USB_USART_APBxCLKCmd    RCC_APB2PeriphClockCmd
#define ROS_USB_USART_GPIO_APBxCLKCmd    RCC_APB2PeriphClockCmd

#define ROS_USB_USART_IRQn         USART1_IRQn
#define ROS_USB_USART_IRHandler    USART1_IRQHandler
#define ros_rdata_size    200

void ROS_USB_USART_Init(void);
void ROS_USB_USART_Send_byte(u8 data);
void ROS_USB_USART_Send_msg(u8 *buf);

void ROS_USB_USART_Send_begin();
void mode_0();
void mode_1();
void flag_Cal();

extern u8 ros_rdata[ros_rdata_size];
extern u16 ros_rdata_len ;       //接收状态标记
extern u8 ans_msg[5];
extern u8 ros_rec_flag;

#endif 
