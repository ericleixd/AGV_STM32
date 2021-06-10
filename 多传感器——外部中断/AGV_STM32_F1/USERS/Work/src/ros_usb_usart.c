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
#include "supersonic.h"
#include "infrared.h"
#include "my_key.h"
u8 ros_rdata[ros_rdata_size];
u16 ros_rdata_len =0;       //接收状态标记
u8 ans_msg[5]={4,0xFF};
u8 ros_rec_flag =0;
u8 mode = 1;
//u8 ultroCount;
u8 ultro_Num = 2;
u8 ir_Num = 2;
u8 flagFront = 0;
u8 flagBack = 0;
u16 min_Dis = 40;
u8 situation = 0x00;
u16 limit_threshold_dis = 50;
u16 braking_threshold_dis = 30;
float limit_threshold_voltage = 0.92;
float braking_threshold_voltage = 2.15;

u8 limit_Situation = 0x00;
u8 ultra_Situation = 0x00;
u8 ir_Situation = 0x00;
u8 mix_Situation = 0x00;

extern uint16_t result[2];
float voltage[6];
float temp = 0;

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
			Res =USART_ReceiveData(ROS_USB_USART);	//读取接收到的数据
			ros_rdata[ros_rdata_len] = Res;
			//ROS_USB_USART_Send_byte(Res);
			ros_rdata_len++;
			if(ros_rdata_len == 3)
			{
				ros_rdata_len = 0;
				mode = ros_rdata[2];
				ROS_USB_USART_Send_begin(); // information head
				// mode -> func
				switch(mode){
					case 0:
						situation = mode_0();
						break;
					case 1:
						situation = mode_1();
						break;
					case 2:
						situation = mode_2();
						break;
					case 4:
						situation = mode_4();
						break;
					case 7:
						situation = mode_7();
						break;
				}
				ROS_USB_USART_Send_byte(situation);
			}
		}
	}
}

u8 mode_0(void)
{
	limit_Situation = 0x00;
	if(limit_Switch_Scan())
	{
		limit_Situation = 0x02;
	}
	//
	return limit_Situation;
}

/*
	mode_1() ultrasonic mode
*/
u8 mode_1(void)
{
	ultra_Situation = 0x00;
	uint8_t i;
	for(i=0;i<ultro_Num;i++)
	{
		Supersonic_Work(i+1);
	}
	for(i=0;i<ultro_Num;i++)
	{
		if(U_dis[i] < limit_threshold_dis)
		{
			ultra_Situation = 0x01;
		}
	}
	for(i=0;i<ultro_Num;i++)
	{
		if(U_dis[i] < braking_threshold_dis)
		{
			ultra_Situation = 0x02;
		}
	}
	return ultra_Situation;
}


u8 mode_2(void)
{
	ir_Situation = 0x00;
	uint8_t n;
	for(n=0;n<ir_Num;n++)
	{
		voltage[n]=(float) result[n]/4096*3.3;
	}
	for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > limit_threshold_voltage)
		{
			ir_Situation = 0x01;
		}
	}
	for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > braking_threshold_voltage)
		{
			ir_Situation = 0x02;
		}
	}
	return ir_Situation;
}

u8 mode_4(void)
{
	mix_Situation = 0x00;

	uint8_t n;
	for(n=0;n<ir_Num;n++)
	{
		voltage[n]=(float) result[n]/4096*3.3;
	}
		for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > limit_threshold_voltage)
		{
			mix_Situation = 0x01;
		}
	}
	for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > braking_threshold_voltage)
		{
			mix_Situation = 0x02;
		}
	}
	
	if(limit_Switch_Scan())
	{
		mix_Situation = 0x02;
	}
	
	return mix_Situation;
}

u8 mode_7(void)
{
	mix_Situation = 0x00;

	uint8_t n;
	for(n=0;n<ir_Num;n++)
	{
		voltage[n]=(float) result[n]/4096*3.3;
	}
		for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > limit_threshold_voltage)
		{
			mix_Situation = 0x01;
		}
	}
	for(n=0;n<ir_Num;n++)
	{
		if(voltage[n] > braking_threshold_voltage)
		{
			mix_Situation = 0x02;
		}
	}
	
	uint8_t i;
	for(i=0;i<ultro_Num;i++)
	{
		Supersonic_Work(i+1);
	}
	for(i=0;i<ultro_Num;i++)
	{
		if(U_dis[i] < braking_threshold_dis)
		{
			mix_Situation = 0x02;
		}
	}
	
	if(limit_Switch_Scan())
	{
		mix_Situation = 0x02;
	}
	
	return mix_Situation;
}


void ROS_USB_USART_Send_begin()
{
	ROS_USB_USART_Send_byte(0xFF);
	ROS_USB_USART_Send_byte(0xAA);
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

		// if IR available -> ultroCount + IRCount
	