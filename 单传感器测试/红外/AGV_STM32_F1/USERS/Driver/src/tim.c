#include "tim.h"

void BASIC_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_Struct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	RCC_APB1PeriphClockCmd(BASIC_TIM_RCC,ENABLE);
	
	//TIM_TimeBase_Struct.TIM_ClockDivision=TIM_CKD_DIV1; //���������ߵ�ʱ�ӽ��з�Ƶ
	TIM_TimeBase_Struct.TIM_Prescaler=7199; //���߷�Ƶ���ٽ���һ��Ԥ��Ƶһ��0.1ms
	//TIM_TimeBase_Struct.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ
	TIM_TimeBase_Struct.TIM_Period=20000; //��������
	
	//TIM_TimeBase_Struct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(BASIC_TIM,&TIM_TimeBase_Struct);
#if BASIC_IT
	NVIC_InitStruct.NVIC_IRQChannel=BASIC_TIM_IRQ;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	
#endif  /*BASIC_IT */
	TIM_Cmd(BASIC_TIM,DISABLE);
	
}




