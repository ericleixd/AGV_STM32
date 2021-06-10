/**
  ******************************************************************************
  * @file    ros_usb_usart.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief  和工控机ROS系统进行串口通信的驱动程序
  ******************************************************************************
  */ 
	
	
#include "ros_usb_usart.h"

u8 ros_rdata[ros_rdata_size];
u16 ros_rdata_len =0;       //接收状态标记
u8 ans_msg[5]={4,0xFF};
u8 ros_rec_flag =0;

void ROS_USB_USART_Init(void)
{
	//结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//时钟
	ROS_USB_USART_APBxCLKCmd(ROS_USB_USART_RX_GPIO_RCC,ENABLE);
	ROS_USB_USART_GPIO_APBxCLKCmd(ROS_USB_USART_RCC,ENABLE);
	//GPIO配置
	//RX为浮空输入
	GPIO_InitStruct.GPIO_Pin=ROS_USB_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(ROS_USB_USART_RX_GPIO_PORT,&GPIO_InitStruct);
	//TX为复用输出
	GPIO_InitStruct.GPIO_Pin=ROS_USB_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ROS_USB_USART_TX_GPIO_PORT,&GPIO_InitStruct);
	//配置通信参数
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ROS_USB_USART,&USART_InitStruct);
	//配置中断
	NVIC_InitStruct.NVIC_IRQChannel=ROS_USB_USART_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//串口中断使能
	USART_ITConfig(ROS_USB_USART,USART_IT_RXNE,ENABLE);
	USART_Cmd(ROS_USB_USART,ENABLE);
	
}

//得到ROS系统发来的消息 关于充电的信息,目前每接收一个字节进入一次中断。
//根据通信协议应该数据长度应该为8位
void ROS_USB_USART_IRHandler(void)
{
	u8 Res;
	if(USART_GetITStatus(ROS_USB_USART,USART_IT_RXNE) != RESET)
	{
		if(USART_GetFlagStatus(ROS_USB_USART,USART_FLAG_RXNE) != RESET)
		{
			USART_ClearFlag(ROS_USB_USART,USART_FLAG_RXNE);
			Res =USART_ReceiveData(ROS_USB_USART);	//读取接收到的数据
			ros_rdata[ros_rdata_len] = Res;
			//ROS_USB_USART_Send_byte(Res);
			ros_rdata_len++;
			if(ros_rdata_len == 5)
			{
				ros_rec_flag=1;
			}
		}
	}
}

//发送数据
void ROS_USB_USART_Send_byte(u8 data)
{
	USART_SendData(ROS_USB_USART,data);
	while(USART_GetFlagStatus(ROS_USB_USART,USART_FLAG_TC) ==RESET);
	USART_ClearFlag(ROS_USB_USART,USART_FLAG_TC);
}


void ROS_USB_USART_Send_msg(u8 *buf)
{
	u8 i;
	for(i=0;i<buf[0]+1;i++)
	{
		ROS_USB_USART_Send_byte(buf[i]);
	}
	
}

	
	