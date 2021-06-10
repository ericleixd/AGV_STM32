
#include "infrared.h"
#include "delay.h"
#include "ros_usb_usart.h"
#include <math.h>

u16 IR_dis[20];
u16 ADC_Value[20];
//int IRNum = 1;
//extern uint16_t result[6];
void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(ADC_GPIO_CLK,  ENABLE);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN ;
	GPIO_InitStruct.GPIO_Pin = ADC_PIN1|
								ADC_PIN2;
	
	GPIO_Init(ADC_PORT , &GPIO_InitStruct);
}

uint16_t result[2]={0,0};

void ADC_DMA_COnfig(void)
{
	ADC_InitTypeDef  ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,  ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* ??DMA1???1 */
	DMA_DeInit(DMA1_Channel1);
	
	// ?? DMA ??????
	// ?????:ADC ???????
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( ADC1_BASE+0x4c);
	
	// ?????
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)result;
	
	// ???????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ?????,????????????
	DMA_InitStructure.DMA_BufferSize = 2;
	
	// ?????????,??????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// ???????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// ?????????,?????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// ??????????,?????????
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ??????
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ?????????,?????DMA???,????????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ???????????,??????????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ???DMA
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// ?? DMA ??
	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	
	// ADC ????
	// ?????ADC,?????
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ????
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// ??????
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// ????????,??????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ???????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ??????
	ADC_InitStructure.ADC_NbrOfChannel = 2;	
		
	// ???ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// ??ADC???PCLK2?8??,?9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ??ADC ????????????
	ADC_RegularChannelConfig(ADC1, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_55Cycles5);
	
	// ??ADC DMA ??
	ADC_DMACmd(ADC1, ENABLE);
	
	// ??ADC ,?????
	ADC_Cmd(ADC1, ENABLE);
	
	// ???ADC ?????  
	ADC_ResetCalibration(ADC1);
	// ????????????
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC????
	ADC_StartCalibration(ADC1);
	// ??????
	while(ADC_GetCalibrationStatus(ADC1));
	
	// ??????????,????????ADC?? 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}












//volatile unsigned long * infrared_addr[1] = {&Infrared_1_IN};



//-------------??? ADC DMA??----------------
void ADC_DMA_Multichannel_Init(u32 ADC_Buff) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
    //??DMA1??
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);      
  
		//???IO 
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;        
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  
    GPIO_Init(GPIOC,&GPIO_InitStructure);  
 
    //??DMA1 ??1 ADC
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;//ADC?????
    DMA_InitStructure.DMA_MemoryBaseAddr  = ADC_Buff;//?????
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//?????
    DMA_InitStructure.DMA_BufferSize = 1;//????
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//????????
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//????????
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//16?? 12?ADC
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//16??
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//????
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//????
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//?????????
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//???DMA1 ??1
    DMA_Cmd(DMA1_Channel1, ENABLE);//??DMA ??1
 
    ADC_DeInit(ADC1);//??ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//????
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //???? ????? 
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //?????? ???? ????
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//??????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//?????
    ADC_InitStructure.ADC_NbrOfChannel = 2;//??3???
    ADC_Init(ADC1, &ADC_InitStructure);//???ADC
		
		//ADC?? ????14MHz
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72/6 = 12MHz 
		//????ADC????,??????
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
		//ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 2, ADC_SampleTime_239Cycles5);
		//ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
    //??ADC DMA
		ADC_DMACmd(ADC1, ENABLE);
		//??ADC
    ADC_Cmd(ADC1, ENABLE);
		
		ADC_ResetCalibration(ADC1);//???????  
    while(ADC_GetResetCalibrationStatus(ADC1));//???????????  
  
    ADC_StartCalibration(ADC1);//ADC??  
    while(ADC_GetCalibrationStatus(ADC1));//??????  
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);//??????????,????????ADC?? 
		
}


 __IO u16 ADCConvertedValue;

/*-------------------GPIO¿ÚµÄÅäÖÃ---------------------*/
void ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN1|ADC_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);
	
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	DMA_DeInit(ADC_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC_x, &ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	//ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC_x, ENABLE);
	ADC_Cmd(ADC_x, ENABLE);
	ADC_ResetCalibration(ADC_x);
	while(ADC_GetResetCalibrationStatus(ADC_x));
	ADC_StartCalibration(ADC_x);
	while(ADC_GetCalibrationStatus(ADC_x));
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}





void read_adc_s_value()
{
	for(int i=0;i<2;i++){
		double sum = 0;
		double temp = 0;
		/*
		for(u8 j=0;j<10;j++){
			sum+=ADCConvertedValue[j][i];
		}
		*/
		
		temp=ADCConvertedValue*(3.3/4096)*10;
		//temp = 147737/(temp * 10);
		//ADC_Value[i] = (u16)pow(temp,1.2134);
		data_IR_dis_pc((u16)temp);
		delay_us(30);
	}

}

void data_IR_dis_pc(u16 dis_buf)
{
	ROS_USB_USART_Send_byte(dis_buf>>8 &0xFF);
	delay_us(30);
	ROS_USB_USART_Send_byte(dis_buf &0xFF);
		
}

