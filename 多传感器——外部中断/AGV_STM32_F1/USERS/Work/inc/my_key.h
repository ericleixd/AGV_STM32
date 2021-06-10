#include "stm32f10x.h"
#include "bitop.h"
#include "delay.h"

// KEY的GPIO所有RCC宏定义
#define limit_Key_RCC  RCC_APB2Periph_GPIOF

#define limit_Key_1_GPIO_Port   GPIOF
#define limit_Key_1_GPIO_Pin    GPIO_Pin_0
#define limit_Key_1_GPIO_RCC    RCC_APB2Periph_GPIOF
#define limit_Key_1_IN     			PFIN(0)

#define limit_Key_2_GPIO_Port   GPIOF
#define limit_Key_2_GPIO_Pin    GPIO_Pin_1
#define limit_Key_2_GPIO_RCC    RCC_APB2Periph_GPIOF
#define limit_Key_2_IN     			PFIN(1)



void limit_Switch_Init(void);

u8 limit_Switch_Scan(void);