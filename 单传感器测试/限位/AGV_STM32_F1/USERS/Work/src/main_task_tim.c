/**
  ******************************************************************************
  * @file    main_task_tim.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief   main函数中处理任务的定时器
  ******************************************************************************
  */ 
	


#include "main_task_tim.h"

u16 count = 0;


/*------------------定时器初始化函数和中断处理函数---------------------*/
	
	void MAIN_TASK_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_Struct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(MAIN_TASK_TIM_RCC,ENABLE);
	
	TIM_TimeBase_Struct.TIM_ClockDivision=TIM_CKD_DIV1; //对来自总线的时钟进行分频 72Mhz
	TIM_TimeBase_Struct.TIM_Prescaler=71; //总线分频完再进行一次预分频  1Mhz一次1us 
	TIM_TimeBase_Struct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBase_Struct.TIM_Period=MAIN_TASK_TIME; 
	TIM_TimeBase_Struct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(MAIN_TASK_TIM,&TIM_TimeBase_Struct);

	NVIC_InitStruct.NVIC_IRQChannel=MAIN_TASK_TIM_IRQ;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=MAIN_TASK_TIM_PrePriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=MAIN_TASK_TIM_SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ClearFlag(MAIN_TASK_TIM,TIM_FLAG_Update);
	TIM_ITConfig(MAIN_TASK_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(MAIN_TASK_TIM,DISABLE);
}


void MAIN_TASK_TIM_IRQHandler()
{
	TIM_Cmd(MAIN_TASK_TIM,DISABLE);
	if(TIM_GetITStatus(MAIN_TASK_TIM,TIM_IT_Update) !=RESET)
	{
		TIM_ClearITPendingBit(MAIN_TASK_TIM,TIM_IT_Update);
		//进行工作模式的刷新
		count +=1;
		data_refresh();
		State_Translate();
		TIM_Cmd(MAIN_TASK_TIM,ENABLE);
			
	}
}

/*------------------定时器初始化函数和中断处理函数---------------------*/

