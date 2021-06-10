#include "uart.h"


void UART_Init(void)
{
	//�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//ʱ��
	USARTx_APBxCLKCmd(USARTx_RX_GPIO_RCC,ENABLE);
	USARTx_GPIO_APBxCLKCmd(USARTx_RCC,ENABLE);
	//GPIO����
	//RXΪ��������
	GPIO_InitStruct.GPIO_Pin=USARTx_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTx_RX_GPIO_PORT,&GPIO_InitStruct);
	//TXΪ�������
	GPIO_InitStruct.GPIO_Pin=USARTx_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USARTx_TX_GPIO_PORT,&GPIO_InitStruct);
	//����ͨ�Ų���
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx,&USART_InitStruct);
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel=USARTx_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//�����ж�ʹ��
	USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);
	USART_Cmd(USARTx,ENABLE);
	
}

void USARTx_IRHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USARTx,USART_IT_RXNE))
	{
		if(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) != RESET)
		{
			USART_ClearFlag(USARTx,USART_FLAG_RXNE);
			res=USART_ReceiveData(USARTx);
			USART_SendData(USARTx,res);
		}
	}
	
}
///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USARTx);

}

