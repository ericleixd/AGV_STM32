/**
  ******************************************************************************
  * @file    battery_state_read.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief  利用库仑计读取电池电量的程序
  ******************************************************************************
  */ 
	
#include "battery_state_read.h"

u8 battery_data[buffer_size];
u8 flag =0;
u8 send_data[8]={0x5A, 0xA5};

void BATTERY_STATE_USART_Init(void)
{
	//结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//时钟
	BATTERY_STATE_USART_APBxCLKCmd(BATTERY_STATE_USART_RX_GPIO_RCC,ENABLE);
	BATTERY_STATE_USART_GPIO_APBxCLKCmd(BATTERY_STATE_USART_RCC,ENABLE);
	//GPIO配置
	//RX为浮空输入
	GPIO_InitStruct.GPIO_Pin=BATTERY_STATE_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(BATTERY_STATE_USART_RX_GPIO_PORT,&GPIO_InitStruct);
	//TX为复用输出
	GPIO_InitStruct.GPIO_Pin=BATTERY_STATE_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BATTERY_STATE_USART_TX_GPIO_PORT,&GPIO_InitStruct);
	//配置通信参数
	USART_InitStruct.USART_BaudRate=9600;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BATTERY_STATE_USART,&USART_InitStruct);
	//配置中断
	NVIC_InitStruct.NVIC_IRQChannel=BATTERY_STATE_USART_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//串口空闲中断使能
	USART_ITConfig(BATTERY_STATE_USART,USART_IT_IDLE,ENABLE);
	
	USART_Cmd(BATTERY_STATE_USART,ENABLE);
	
}

void BATTERY_STATE_Rx_DMA_Config(u32 memoryAddr, u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(BATTERY_STATE_USART_RX_DMA_CHANEL);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = BATTERY_STATE_USART_DR_Addr;//
	DMA_InitStructure.DMA_MemoryBaseAddr = memoryAddr;
	DMA_InitStructure.DMA_BufferSize = BufferSize;									//数据传输量
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //传输方向
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//储存器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;			//外设数据长度，1Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//储存器数据长度，1Byte
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;									//正常模式，非循环采集
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//高优先级
	DMA_Init(BATTERY_STATE_USART_RX_DMA_CHANEL, &DMA_InitStructure);
	
	DMA_Cmd(BATTERY_STATE_USART_RX_DMA_CHANEL, ENABLE);
	//串口接收DMA使能
	USART_DMACmd(BATTERY_STATE_USART, USART_DMAReq_Rx, ENABLE);
	uint8_t rc_tmp = BATTERY_STATE_USART->SR;
  rc_tmp=BATTERY_STATE_USART->DR;		
}

void BATTERY_STATE_USART_IRHandler(void)
{
  u8 len;	
	if(USART_GetITStatus(BATTERY_STATE_USART,USART_IT_IDLE) !=RESET)
	{
		//软件清除标志位
		len = BATTERY_STATE_USART->SR;
    len=BATTERY_STATE_USART->DR;
		//关闭DMA
		DMA_Cmd(BATTERY_STATE_USART_RX_DMA_CHANEL, DISABLE);		
		len=DMA_GetCurrDataCounter(BATTERY_STATE_USART_RX_DMA_CHANEL);		
		DMA_SetCurrDataCounter(BATTERY_STATE_USART_RX_DMA_CHANEL,buffer_size); //设置数据传输量
		DMA_Cmd(BATTERY_STATE_USART_RX_DMA_CHANEL, ENABLE);		
		flag=1;
	}
	
}




void BATTERY_STATE_USART_Send_byte(u8 data)
{
	USART_SendData(BATTERY_STATE_USART,data);
	while(USART_GetFlagStatus(BATTERY_STATE_USART,USART_FLAG_TC) ==RESET);
	USART_ClearFlag(BATTERY_STATE_USART,USART_FLAG_TC);
	
}

void BATTERY_STATE_USART_Send_msg(u8 *buf)
{
	u8 i;
	for(i=0;i<buf[0]+1;i++)
	{
		BATTERY_STATE_USART_Send_byte(buf[i]);
	}
	
}


void battery_data_pkg(u8 mode)
{
	
	switch(mode)
	{
		case 0x10:
			send_data[2]=0x10;
			send_data[3]=0;
			send_data[4]=0;
			send_data[5]=0;
			send_data[6]=0;
			send_data[7]=0x0F;
			break;
		
	}
}






