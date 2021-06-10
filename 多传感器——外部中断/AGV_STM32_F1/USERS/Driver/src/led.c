
#include "led.h"

//void LED_Init(void)
//{
// 
// GPIO_InitTypeDef  GPIO_InitStructure;
// 	
// RCC_APB2PeriphClockCmd(G1LED_GPIO_RCC|G2LED_GPIO_RCC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
//	
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
// 						
// GPIO_InitStructure.GPIO_Pin = G1LED_GPIO_PIN;	    		 //LED1-->PE.5 �˿�����, �������
// GPIO_Init(G1LED_GPIO_PORT, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
//	
// GPIO_InitStructure.GPIO_Pin = G2LED_GPIO_PIN;	    		 
// GPIO_Init(G2LED_GPIO_PORT, &GPIO_InitStructure);	 
//	
//}

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=LED1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=LED2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
}
