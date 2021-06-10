#include "uart.h"


void UART_Init(void)
{
	//结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//时钟
	USARTx_APBxCLKCmd(USARTx_RX_GPIO_RCC,ENABLE);
	USARTx_GPIO_APBxCLKCmd(USARTx_RCC,ENABLE);
	//GPIO配置
	//RX为浮空输入
	GPIO_InitStruct.GPIO_Pin=USARTx_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTx_RX_GPIO_PORT,&GPIO_InitStruct);
	//TX为复用输出
	GPIO_InitStruct.GPIO_Pin=USARTx_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(USARTx_TX_GPIO_PORT,&GPIO_InitStruct);
	//配置通信参数
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx,&USART_InitStruct);
	//配置中断
	NVIC_InitStruct.NVIC_IRQChannel=USARTx_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//串口中断使能
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
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USARTx);

}

