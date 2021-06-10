#ifndef __KEY_H
#define  __KEY_H

#include "stm32f10x.h"
#include "bitop.h"
#include "delay.h"

#define KEY1_GPIO_PIN   GPIO_Pin_3
#define KEY1_GPIO_PORT  GPIOE
#define KEY1_GPIO_RCC   RCC_APB2Periph_GPIOE
//#define KEY1            PEIN(3)
#define KEY1_PRE         2

#define KEY2_GPIO_PIN   GPIO_Pin_0
#define KEY2_GPIO_PORT  GPIOA
#define KEY2_GPIO_RCC   RCC_APB2Periph_GPIOA
#define KEY2            PAIN(0)
#define KEY2_PRE          3

#define KEY0_GPIO_PIN   GPIO_Pin_4
#define KEY0_GPIO_PORT  GPIOE
#define KEY0_GPIO_RCC   RCC_APB2Periph_GPIOE
//#define KEY0            PEIN(4)
#define KEY0_PRE           1

#define KEY0     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 


void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
