
#include "stm32f10x.h"
#include "bitop.h"
#include "stm32f10x_adc.h"
//  Infrared GPIO所有RCC宏定义

#define    NOFCHANEL
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1
 
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC



#define    ADC_PIN1                      GPIO_Pin_4
#define    ADC_CHANNEL1                  ADC_Channel_14 // PA2
 
#define    ADC_PIN2                      GPIO_Pin_5
#define    ADC_CHANNEL2                  ADC_Channel_15 // PA3


#define ADC_x                  ADC1
#define ADC_DMA_CHANNEL        DMA1_Channel1
#define ADC_DMA_CLK      		 	 RCC_AHBPeriph_DMA1

#define Infrared_1_GPIO_Port   GPIOC
#define Infrared_1_GPIO_Pin    ADC_PIN1
#define Infrared_1_GPIO_RCC    RCC_APB2Periph_GPIOC
#define Infrared_1_IN          PCIN(4)

#define Infrared_2_GPIO_Port   GPIOA
#define Infrared_2_GPIO_Pin    ADC_PIN2
#define Infrared_2_GPIO_RCC    RCC_APB2Periph_GPIOC
#define Infrared_2_IN          PCIN(5)


#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1

void ADCx_Init(void);
void ADC_GPIO_Config(void);
void ADC_DMA_COnfig(void);
void read_adc_s_value();
void data_IR_dis_pc(u16 dis_buf);
void ADC_DMA_Multichannel_Init(u32 ADC_Buff) ;
extern u16 ADC_Value[20];// save Infrared distance
extern u16 IR_dis[20];// save Infrared distance
extern uint16_t result[2];
