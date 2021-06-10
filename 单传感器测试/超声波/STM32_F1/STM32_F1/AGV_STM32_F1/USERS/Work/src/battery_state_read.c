/**
  ******************************************************************************
  * @file    battery_state_read.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief  ���ÿ��ؼƶ�ȡ��ص����ĳ���
  ******************************************************************************
  */ 
	
#include "battery_state_read.h"

u8 battery_data[buffer_size];
u8 flag =0;
u8 send_data[8]={0x5A, 0xA5};

void BATTERY_STATE_USART_Init(void)
{
	//�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//ʱ��
	BATTERY_STATE_USART_APBxCLKCmd(BATTERY_STATE_USART_RX_GPIO_RCC,ENABLE);
	BATTERY_STATE_USART_GPIO_APBxCLKCmd(BATTERY_STATE_USART_RCC,ENABLE);
	//GPIO����
	//RXΪ��������
	GPIO_InitStruct.GPIO_Pin=BATTERY_STATE_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(BATTERY_STATE_USART_RX_GPIO_PORT,&GPIO_InitStruct);
	//TXΪ�������
	GPIO_InitStruct.GPIO_Pin=BATTERY_STATE_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BATTERY_STATE_USART_TX_GPIO_PORT,&GPIO_InitStruct);
	//����ͨ�Ų���
	USART_InitStruct.USART_BaudRate=9600;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BATTERY_STATE_USART,&USART_InitStruct);
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel=BATTERY_STATE_USART_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//���ڿ����ж�ʹ��
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
	DMA_InitStructure.DMA_BufferSize = BufferSize;									//���ݴ�����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;              //���䷽��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//����������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;			//�������ݳ��ȣ�1Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//���������ݳ��ȣ�1Byte
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;									//����ģʽ����ѭ���ɼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;								//�����ȼ�
	DMA_Init(BATTERY_STATE_USART_RX_DMA_CHANEL, &DMA_InitStructure);
	
	DMA_Cmd(BATTERY_STATE_USART_RX_DMA_CHANEL, ENABLE);
	//���ڽ���DMAʹ��
	USART_DMACmd(BATTERY_STATE_USART, USART_DMAReq_Rx, ENABLE);
	uint8_t rc_tmp = BATTERY_STATE_USART->SR;
  rc_tmp=BATTERY_STATE_USART->DR;		
}

void BATTERY_STATE_USART_IRHandler(void)
{
  u8 len;	
	if(USART_GetITStatus(BATTERY_STATE_USART,USART_IT_IDLE) !=RESET)
	{
		//��������־λ
		len = BATTERY_STATE_USART->SR;
    len=BATTERY_STATE_USART->DR;
		//�ر�DMA
		DMA_Cmd(BATTERY_STATE_USART_RX_DMA_CHANEL, DISABLE);		
		len=DMA_GetCurrDataCounter(BATTERY_STATE_USART_RX_DMA_CHANEL);		
		DMA_SetCurrDataCounter(BATTERY_STATE_USART_RX_DMA_CHANEL,buffer_size); //�������ݴ�����
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






