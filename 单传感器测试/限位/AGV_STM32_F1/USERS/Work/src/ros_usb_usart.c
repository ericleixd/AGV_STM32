/**
  ******************************************************************************
  * @file    ros_usb_usart.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief  �͹��ػ�ROSϵͳ���д���ͨ�ŵ���������
  ******************************************************************************
  */ 
	
	
#include "ros_usb_usart.h"
#include "supersonic.h"
u8 ros_rdata[ros_rdata_size];
u16 ros_rdata_len =0;       //����״̬���
u8 ans_msg[5]={4,0xFF};
u8 ros_rec_flag =0;
u8 mode = 1;
//u8 ultroCount;
u8 sensor_Num;
u8 flagFront = 0;
u8 flagBack = 0;
u16 min_Dis = 40;


void ROS_USB_USART_Init(void)
{
	//�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//ʱ��
	ROS_USB_USART_APBxCLKCmd(ROS_USB_USART_RX_GPIO_RCC,ENABLE);
	ROS_USB_USART_GPIO_APBxCLKCmd(ROS_USB_USART_RCC,ENABLE);
	//GPIO����
	//RXΪ��������
	GPIO_InitStruct.GPIO_Pin=ROS_USB_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(ROS_USB_USART_RX_GPIO_PORT,&GPIO_InitStruct);
	//TXΪ�������
	GPIO_InitStruct.GPIO_Pin=ROS_USB_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ROS_USB_USART_TX_GPIO_PORT,&GPIO_InitStruct);
	//����ͨ�Ų���
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ROS_USB_USART,&USART_InitStruct);
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel=ROS_USB_USART_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//�����ж�ʹ��
	USART_ITConfig(ROS_USB_USART,USART_IT_RXNE,ENABLE);
	USART_Cmd(ROS_USB_USART,ENABLE);
	
}

//�õ�ROSϵͳ��������Ϣ ���ڳ�����Ϣ,Ŀǰÿ����һ���ֽڽ���һ���жϡ�
//����ͨ��Э��Ӧ�����ݳ���Ӧ��Ϊ8λ


void ROS_USB_USART_IRHandler(void)
{
	u8 Res;
	 if (USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)  
   {  
       USART_ReceiveData(ROS_USB_USART);  
     USART_ClearFlag(ROS_USB_USART, USART_FLAG_PE);  
   }  
      
   if (USART_GetFlagStatus(ROS_USB_USART, USART_FLAG_ORE) != RESET)  
   {  
       USART_ReceiveData(ROS_USB_USART);  
     USART_ClearFlag(ROS_USB_USART, USART_FLAG_ORE);  
   }  
      
   if (USART_GetFlagStatus(ROS_USB_USART, USART_FLAG_FE) != RESET)  
   {  
       USART_ReceiveData(ROS_USB_USART);  
      USART_ClearFlag(ROS_USB_USART, USART_FLAG_FE);  
   }  
      
	if(USART_GetITStatus(ROS_USB_USART,USART_IT_RXNE) != RESET)
	{
		if(USART_GetFlagStatus(ROS_USB_USART,USART_FLAG_RXNE) != RESET)
		{
			USART_ClearFlag(ROS_USB_USART,USART_FLAG_RXNE);
			Res =USART_ReceiveData(ROS_USB_USART);	//��ȡ���յ�������
			ros_rdata[ros_rdata_len] = Res;
			//ROS_USB_USART_Send_byte(Res);
			ros_rdata_len++;
			if(ros_rdata_len == 3)
			{
				ros_rdata_len = 0;
				mode = ros_rdata[2];
				// mode -> func
				switch(mode){
					case 0:
						mode_0();
					case 1:
						mode_1();
				}
			}
		}
	}
}

void mode_0()
{
	ROS_USB_USART_Send_begin(); // information head
	
}

/*
	mode_1() ultrasonic mode
*/
void mode_1()
{
	ROS_USB_USART_Send_begin(); // information head
	ROS_USB_USART_Send_byte(0x00);// limit switch disable
	sensor_Num = ultroCount;
	//ROS_USB_USART_Send_byte(sensor_Num); // number of ultrasonic sensor
	// send the distances tested by ultrasonic sensors respectively
	flagFront = 0;
	flagBack = 0;
	for(int i =0; i<((int) sensor_Num);i++)
	{
		Supersonic_Work(i+1);
		//ROS_USB_USART_Send_byte(U_dis[i]>>8 &0xFF);
		//ROS_USB_USART_Send_byte(U_dis[i] &0xFF);
	}
	flag_Cal();
	ROS_USB_USART_Send_byte(flagFront);
	ROS_USB_USART_Send_byte(flagBack);
}

void flag_Cal()
{
	if(U_dis[0] < 2 * min_Dis || U_dis[3] < 2 * min_Dis || U_dis[1] < 3 * min_Dis || U_dis[4] < 3 * min_Dis)
	{
		flagFront = 1;
		
		if(U_dis[0] < min_Dis || U_dis[3] < min_Dis  || U_dis[1] <  min_Dis || U_dis[4] < min_Dis)
		{
			flagFront = 2;
		}
	}
	
	if(U_dis[2] < min_Dis || U_dis[5] < min_Dis)
	{
		flagBack = 1;
	}
}



void ROS_USB_USART_Send_begin()
{
	ROS_USB_USART_Send_byte(0xFF);
	ROS_USB_USART_Send_byte(0xAA);
}

//��������
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

		// if IR available -> ultroCount + IRCount
	