#include "stm32f10x.h"
#include "bitop.h"
#include "delay.h"

// KEY的GPIO所有RCC宏定义
#define limit_Key_RCC  RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOC

#define limit_Key_1_GPIO_Port   GPIOF
#define limit_Key_1_GPIO_Pin    GPIO_Pin_0
#define limit_Key_1_GPIO_RCC    RCC_APB2Periph_GPIOF
#define limit_Key_1_IN     			PFIN(0)

#define limit_Key_2_GPIO_Port   GPIOF
#define limit_Key_2_GPIO_Pin    GPIO_Pin_1
#define limit_Key_2_GPIO_RCC    RCC_APB2Periph_GPIOF
#define limit_Key_2_IN     			PFIN(1)

#define limit_Key_3_GPIO_Port   GPIOC
#define limit_Key_3_GPIO_Pin    GPIO_Pin_4
#define limit_Key_3_GPIO_RCC    RCC_APB2Periph_GPIOC
#define limit_Key_3_IN     			PCIN(4)

#define limit_Key_4_GPIO_Port   GPIOC
#define limit_Key_4_GPIO_Pin    GPIO_Pin_5
#define limit_Key_4_GPIO_RCC    RCC_APB2Periph_GPIOC
#define limit_Key_4_IN     			PCIN(5)



void limit_Key_Init(void);
u8 limit_Key_Scan(void);