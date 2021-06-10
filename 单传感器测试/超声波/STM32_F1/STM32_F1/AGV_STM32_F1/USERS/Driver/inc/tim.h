#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"

#define BASIC_TIM6   
#define BASIC_IT     1

#ifdef BASIC_TIM6
#define BASIC_TIM             TIM6
#define BASIC_TIM_RCC         RCC_APB1Periph_TIM6
#define BASIC_TIM_IRQ         TIM6_IRQn
#define BASIC_TIM_IRQHandler  TIM6_IRQHandler

void BASIC_TIM_Init(void);

#else
#define BASIC_TIM             TIM7
#define BASIC_TIM_RCC         RCC_APB1Periph_TIM7
#define BASIC_TIM_IRQ         TIM7_IRQn
#define BASIC_TIM_IRQHandler  TIM7_IRQHandler
#endif     /*BASIC_TIM6*/
#endif  /* __TIM_H*/


