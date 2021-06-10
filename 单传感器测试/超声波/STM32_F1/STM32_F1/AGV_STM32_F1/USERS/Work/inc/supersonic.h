#ifndef SUPERSONIC_H
#define SUPERSONIC_H

#include "stm32f10x.h"
#include "bitop.h"

// ³¬Éù²¨µÄGPIOËùÓÐRCCºê¶¨Òå
#define Supersonic_RCC  RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG

//1  PE13 PE14
#define Supersonic_1_TRIG_GPIO_Port   GPIOE
#define Supersonic_1_TRIG_GPIO_Pin    GPIO_Pin_13
#define Supersonic_1_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOE
#define Supersonic_1_TRIG_OUT         PEOUT(13)

#define Supersonic_1_ECHO_GPIO_Port   GPIOE
#define Supersonic_1_ECHO_GPIO_Pin    GPIO_Pin_14
#define Supersonic_1_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOE
#define Supersonic_1_ECHO_IN          PEIN(14)
//2 PF12 PB0
#define Supersonic_2_TRIG_GPIO_Port   GPIOF
#define Supersonic_2_TRIG_GPIO_Pin    GPIO_Pin_12
#define Supersonic_2_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOF
#define Supersonic_2_TRIG_OUT         PFOUT(12)

#define Supersonic_2_ECHO_GPIO_Port   GPIOB
#define Supersonic_2_ECHO_GPIO_Pin    GPIO_Pin_0
#define Supersonic_2_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOB
#define Supersonic_2_ECHO_IN          PBIN(0)
//3 PF11 PB1
#define Supersonic_3_TRIG_GPIO_Port   GPIOF
#define Supersonic_3_TRIG_GPIO_Pin    GPIO_Pin_11
#define Supersonic_3_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOF
#define Supersonic_3_TRIG_OUT         PFOUT(11)

#define Supersonic_3_ECHO_GPIO_Port   GPIOB
#define Supersonic_3_ECHO_GPIO_Pin    GPIO_Pin_1
#define Supersonic_3_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOB
#define Supersonic_3_ECHO_IN          PBIN(1)
//4 PF4 PD3
#define Supersonic_4_TRIG_GPIO_Port   GPIOF
#define Supersonic_4_TRIG_GPIO_Pin    GPIO_Pin_4
#define Supersonic_4_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOF
#define Supersonic_4_TRIG_OUT         PFOUT(4)

#define Supersonic_4_ECHO_GPIO_Port   GPIOD
#define Supersonic_4_ECHO_GPIO_Pin    GPIO_Pin_3
#define Supersonic_4_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOD
#define Supersonic_4_ECHO_IN          PDIN(3)
//5 PF3 PD4
#define Supersonic_5_TRIG_GPIO_Port   GPIOF
#define Supersonic_5_TRIG_GPIO_Pin    GPIO_Pin_3
#define Supersonic_5_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOF
#define Supersonic_5_TRIG_OUT         PFOUT(3)

#define Supersonic_5_ECHO_GPIO_Port   GPIOD
#define Supersonic_5_ECHO_GPIO_Pin    GPIO_Pin_4
#define Supersonic_5_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOD
#define Supersonic_5_ECHO_IN          PDIN(4)
//6 PF2 PD5
#define Supersonic_6_TRIG_GPIO_Port   GPIOF
#define Supersonic_6_TRIG_GPIO_Pin    GPIO_Pin_2
#define Supersonic_6_TRIG_GPIO_RCC    RCC_APB2Periph_GPIOF
#define Supersonic_6_TRIG_OUT         PFOUT(2)

#define Supersonic_6_ECHO_GPIO_Port   GPIOD
#define Supersonic_6_ECHO_GPIO_Pin    GPIO_Pin_5
#define Supersonic_6_ECHO_GPIO_RCC    RCC_APB2Periph_GPIOD
#define Supersonic_6_ECHO_IN          PDIN(5)

#define Supersonic_TIM             TIM6
#define Supersonic_TIM_RCC         RCC_APB1Periph_TIM6
#define Supersonic_TIM_IRQ         TIM6_IRQn
#define Supersonic_TIM_IRQHandler  TIM6_IRQHandler
#define Supersonic_TIM_PrePriority    0
#define Supersonic_TIM_SubPriority    1


#define Supersonic_Count_TIME 1000-1
void Supersonic_TIM_Init(void);
void Supersonic_TIM_Open(void);
void Supersonic_TIM_Close(void);
u32 GetEchoTime(void);
u8 Supersonic_Work(int Super_Num);
void Supersonic_GPIO_Init(void);
void data_send_pc(u16 dis_buf);
extern u16 U_dis[20];//³¬Éù²¨¾àÀë´æ´¢
extern int U_t[10];



#endif  /*  SUPERSONIC_H*/
