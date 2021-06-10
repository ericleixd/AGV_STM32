
#include "charge_232_usart.h"


u8 rData1[Bufsize] = {0};
u8 rData2[Bufsize] = {0};
u8 rIndex1 = 0;

u8 sIndexA = 0xFF;  //数据索引 为FF时意味着数据发送完成，能进行下一次数据打包发送
u8 sdataA[Bufsize] = {0}; //发送给充电板的打包好的数据保存

u8 batrSOC[8] = {0,60,0,0,0,0,0,15};
u8 positionXY[4] = {0,11,0,9};
u8 addrs[4] = {0,1,0,2};

//充电的信息
Charge_Mode charge_mode = Constant_V_MODE;
float Uref=24;
float	Iref=3 ;

u8 handleflag1 = 0;
u8 infoRecv[10] = {0,30,0,0,0,0,0,25,0,0};

u8 chgmode[3] = {1,0,0};  // 0:mode:1:U 2:I  1-2: Uref or Iref
u8 temp_comm = 0x00;
 



void usart2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART2 TX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出；
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //端口A；
	    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART2 RX；
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入；
	GPIO_Init(GPIOA, &GPIO_InitStructure); //端口A；
	
	USART_InitStructure.USART_BaudRate = 115200; //波特率；
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位；
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位1位；
	USART_InitStructure.USART_Parity = USART_Parity_No ; //无校验位；
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//无硬件流控；
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//收发模式；
	USART_Init(USART2, &USART_InitStructure);//配置串口参数；
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置中断组，4位抢占优先级，4位响应优先级；
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //中断号；
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级；
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级；
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE); //使能串口；
}


void USART2_Send_Byte(u8 Data) //发送一个字节；
{
	USART_SendData(USART2,Data);
	while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
}


void USART2_Send_Data(u8 *Data) //发送一个字节；
{
	USART2_Send_Byte(Data[0]);
	USART2_Send_Byte(Data[1]);
	USART2_Send_Byte(Data[2]);
}


void Send_SCIA(void)
{
	USART2_Send_Byte(sdataA[sIndexA]);
	//ROS_USB_USART_Send_byte(sdataA[sIndexA]);
	sIndexA++;
	if (sIndexA >= sdataA[2])
	{
		sIndexA = 0x00FF;
	}
}

//对发送的数据进行编码 
void Charge_Data_Pack(char type)
{
	int i = 0;
	int exmmsg = 0;
	int length = 0;
	if(sIndexA == 0xFF)
	{
		switch(type)
		{
		case 0x01://
		{
			length = 13;
			sdataA[i++] = 0xAA;
			sdataA[i++] = 0x55;
			sdataA[i++] = length;
			sdataA[i++] = type;
			sdataA[i++] = 0;
			sdataA[i++] = 0;
			sdataA[i++] = 0;
			sdataA[i++] = 0;
			sdataA[i++] = batrSOC[0]; // SOC
			sdataA[i++] = batrSOC[1];
			sdataA[i++] = batrSOC[6]; // 
			sdataA[i++] = batrSOC[7];
			exmmsg = sdataA[length-2] + sdataA[length-3] + sdataA[length-4] + sdataA[length-5];
			exmmsg &= 0x00ff;
			sdataA[i++] = exmmsg;
			sIndexA = 0x00;
			break;
		}
		case 0x03:
		{
			length = 17;
			sdataA[i++] = 0xAA;
			sdataA[i++] = 0x55;
			sdataA[i++] = length;
			sdataA[i++] = type;
			sdataA[i++] = positionXY[0]; //
			sdataA[i++] = positionXY[1];
			sdataA[i++] = positionXY[2];
			sdataA[i++] = positionXY[3];
			sdataA[i++] = addrs[0];
			sdataA[i++] = addrs[1];
			sdataA[i++] = addrs[2];
			sdataA[i++] = addrs[3]; 
			//u8 chgmode[6] = {0,0,0};  // 0:mode:1:U 2:I  1-2: Uref or Iref
			sdataA[i++] = charge_mode;
			if(charge_mode == Constant_V_MODE)
			{
				chgmode[1]=(int)Uref;
				chgmode[2]= (int)((Uref-chgmode[1])*100);
			}else
			{
				chgmode[1]=(int)Iref;
				chgmode[2]= (int)((Iref-chgmode[1])*100);
			}
			sdataA[i++] = chgmode[1]&0xff;
			sdataA[i++] = chgmode[2]&0xff;
			sdataA[i++] = 0x11;
			exmmsg = sdataA[length-2] + sdataA[length-3] + sdataA[length-4] + sdataA[length-5];
			exmmsg &= 0x00ff;
			sdataA[i++] = exmmsg;
			sIndexA = 0x00;
			break;
		}
		case 0x11:
		{
			length = 6;
			sdataA[i++] = 0xAA; 
			sdataA[i++] = 0x55;
			sdataA[i++] = length;
			sdataA[i++] = type;
			sdataA[i++] = type;
			exmmsg = sdataA[length-2] + sdataA[length-3] + sdataA[length-4] + sdataA[length-5];
			exmmsg &= 0x00ff;
			sdataA[i++] = exmmsg;
			sIndexA = 0x00;
			break;
		}
		case 0x12:
		{
			length = 6;
			sdataA[i++] = 0xAA;
			sdataA[i++] = 0x55;
			sdataA[i++] = length;
			sdataA[i++] = type;
			sdataA[i++] = type;
			exmmsg = sdataA[length-2] + sdataA[length-3] + sdataA[length-4] + sdataA[length-5];
			exmmsg &= 0x00ff;
			sdataA[i++] = exmmsg;
			sIndexA = 0x00;
			break;
		}
		default:
			break;
		}
	}
}








