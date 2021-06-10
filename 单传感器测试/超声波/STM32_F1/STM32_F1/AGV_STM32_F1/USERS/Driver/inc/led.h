#ifndef __LED_H
#define  __LED_H

#include "stm32f10x.h"
#include "bitop.h"



#define G2LED_GPIO_PIN   GPIO_Pin_5
#define G2LED_GPIO_PORT  GPIOE
#define G2LED_GPIO_RCC   RCC_APB2Periph_GPIOE

#define G1LED_GPIO_PIN   GPIO_Pin_9
#define G1LED_GPIO_PORT  GPIOC
#define G1LED_GPIO_RCC   RCC_APB2Periph_GPIOC

#define LED1 GPIO_Pin_5
#define LED2 GPIO_Pin_9

//位带操作
#define RLED    PBOUT(5)
#define G2LED   PEOUT(5)
#define G1LED   PCOUT(9)




void LED_Init(void);

//异或运算符 与1异或要改变 与0异或不变
#define LED_OVERTURN     {LED_GPIO_PORT->ODR ^=0x01}

#endif
