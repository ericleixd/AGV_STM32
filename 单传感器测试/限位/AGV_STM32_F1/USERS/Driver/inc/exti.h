#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"
#include "bitop.h"
#define KEY_INT_GPIO_PIN  GPIO_Pin_0
#define KEY_INT_GPIO_PORT  GPIOA
#define KEY_INT_GPIO_RCC  RCC_APB2Periph_GPIOA


void EXTI_Key_Init(void);
#endif   /* __EXTI_H*/
