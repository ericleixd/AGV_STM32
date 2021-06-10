#ifndef __MAIN_TASK_TIM_H
#define __MAIN_TASK_TIM_H

#include "stm32f10x.h"
#include "main_task.h"


#define MAIN_TASK_TIM              TIM5
#define MAIN_TASK_TIM_RCC          RCC_APB1Periph_TIM5
#define MAIN_TASK_TIM_IRQ          TIM5_IRQn
#define MAIN_TASK_TIM_IRQHandler   TIM5_IRQHandler
#define  MAIN_TASK_TIM_PrePriority  0
#define  MAIN_TASK_TIM_SubPriority  1


#define MAIN_TASK_TIME  2000-1


void MAIN_TASK_TIM_Init(void);
extern u16 count;


#endif  /*__MAIN_TASK_TIM_H*/

