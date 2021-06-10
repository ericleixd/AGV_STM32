/*
�������źŶ�ȡ��Ŀǰʹ����ͨ��������ȡ�ġ�
���ֺ�����֮ǰд�ģ����ⲿ�жϻ�ó������ķ������ݡ�
������������������
TRIG���ţ���������������������߷���һ��ʱ��20us�����壬���������յ��ᷢ�����塢
ECHO���ţ������������������󣬸����ŵ�ƽ�ɵ����ߣ�Ҳ��˵ECHO����ͨ��Ϊ�͵�ƽ������ģʽӦ������Ϊ��������/�������롣
*/

#include "supersonic.h"
#include "delay.h"
#include "ros_usb_usart.h"
#include "led.h"
//��������Ҫ�ı���
int ultroNum = 6;
volatile unsigned long * supersonic_trig_addr[6] = {&Supersonic_1_TRIG_OUT,&Supersonic_2_TRIG_OUT,&Supersonic_3_TRIG_OUT,
																										&Supersonic_4_TRIG_OUT,&Supersonic_5_TRIG_OUT,&Supersonic_6_TRIG_OUT};
volatile unsigned long * supersonic_echo_addr[6] = {&Supersonic_1_ECHO_IN,&Supersonic_2_ECHO_IN,&Supersonic_3_ECHO_IN,
																										&Supersonic_4_ECHO_IN,&Supersonic_5_ECHO_IN,&Supersonic_6_ECHO_IN};

int ultroCount;//��������ʱ�����Ի�ȡ���ĳ���������
int U_start[10];//����������ʱ��
int U_end[10];//����������ʱ��
int U_t[10];//����������ʱ��
u8 U_sign[10];//���ڼ�¼ÿ���������Ƿ�����
u16 U_dis[20];//����������洢
u8 IsUltroWrong = 0;//�ܵļ�¼�Ƿ��г������쳣
u8 ultrasonic_ALLOW = 0;
int maxTime = 40000;

u32 msCount;

/*-------------------GPIO�ڵ�����---------------------*/

void Supersonic_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Supersonic_RCC, ENABLE);
	
	/*--------- TRIG�ڵ�����--------- */
	//��һ��
	GPIO_InitStructure.GPIO_Pin = Supersonic_1_TRIG_GPIO_Pin;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(Supersonic_1_TRIG_GPIO_Port, &GPIO_InitStructure);
	//������������õ�PF��
	GPIO_InitStructure.GPIO_Pin = Supersonic_2_TRIG_GPIO_Pin | Supersonic_3_TRIG_GPIO_Pin| Supersonic_4_TRIG_GPIO_Pin | Supersonic_5_TRIG_GPIO_Pin | Supersonic_6_TRIG_GPIO_Pin;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(Supersonic_2_TRIG_GPIO_Port, &GPIO_InitStructure);
	
	/*---------ECHO�ڵ�����---------*/
	GPIO_InitStructure.GPIO_Pin = Supersonic_1_ECHO_GPIO_Pin;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������� ���������Ҫ�޸�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(Supersonic_1_ECHO_GPIO_Port, &GPIO_InitStructure);
	//2��3�õ�PB��
	GPIO_InitStructure.GPIO_Pin = Supersonic_2_ECHO_GPIO_Pin | Supersonic_3_ECHO_GPIO_Pin;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������� ���������Ҫ�޸�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(Supersonic_2_ECHO_GPIO_Port, &GPIO_InitStructure);
	//4 5 6�õ�PD��
	GPIO_InitStructure.GPIO_Pin = Supersonic_4_ECHO_GPIO_Pin | Supersonic_5_ECHO_GPIO_Pin | Supersonic_6_ECHO_GPIO_Pin;				 //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������� ���������Ҫ�޸�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(Supersonic_4_ECHO_GPIO_Port, &GPIO_InitStructure);
	for(int i =0;i<6;i++)
	{
		*supersonic_trig_addr[i]=0;
	}
}

/*-------------------��ʱ��������---------------------*/

void Supersonic_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBase_Struct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(Supersonic_TIM_RCC,ENABLE);
	
	TIM_TimeBase_Struct.TIM_ClockDivision=TIM_CKD_DIV1; //���������ߵ�ʱ�ӽ��з�Ƶ 72Mhz
	TIM_TimeBase_Struct.TIM_Prescaler=71; //���߷�Ƶ���ٽ���һ��Ԥ��Ƶ  1Mhzһ��1us 
	TIM_TimeBase_Struct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBase_Struct.TIM_Period=Supersonic_Count_TIME; 
	
	TIM_TimeBase_Struct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(Supersonic_TIM,&TIM_TimeBase_Struct);

	NVIC_InitStruct.NVIC_IRQChannel=Supersonic_TIM_IRQ;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=Supersonic_TIM_PrePriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=Supersonic_TIM_SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ClearFlag(Supersonic_TIM,TIM_FLAG_Update);
	TIM_ITConfig(Supersonic_TIM,TIM_IT_Update,ENABLE);
	TIM_Cmd(Supersonic_TIM,DISABLE);
}

void Supersonic_TIM_Open(void)
{
	TIM_SetCounter(Supersonic_TIM, 0);
	msCount = 0;
	TIM_Cmd(Supersonic_TIM, ENABLE);	
}

void Supersonic_TIM_Close(void)
{
	TIM_Cmd(Supersonic_TIM, DISABLE);	
}

void Supersonic_TIM_IRQHandler(void)
{
	if(TIM_GetITStatus(Supersonic_TIM,TIM_IT_Update) !=RESET)
	{
		TIM_ClearITPendingBit(Supersonic_TIM,TIM_IT_Update);
		msCount++;
	}
}

u32 GetEchoTime(void)
{
	u32 t =0;
	t = msCount*1000 + TIM_GetCounter(Supersonic_TIM);
	//Supersonic_TIM->CNT = 0;
	return t;
}

u8 Supersonic_Work(int Ultra_num)
{
	int Super_Num = Ultra_num-1;
	U_start[Super_Num]=0;
	U_end[Super_Num]=0;
	*supersonic_trig_addr[Super_Num]=1; 
	delay_us(20);
	Supersonic_TIM_Close();
	Supersonic_TIM_Open();
	*supersonic_trig_addr[Super_Num] = 0;
/*
	for(int i =0; i<6;i++)
	{	
		if(*supersonic_echo_addr[Super_Num]==1)
		{
			break;
		}
		delay_us(10);
		if(i==6)
		{
			return 0;
		}
	}
*/
	while(*supersonic_echo_addr[Super_Num]==0);
	/*
	{
		//�ж���û�г�����
		
	}
	*/
	U_start[Super_Num] = GetEchoTime();
	delay_us(60);
	U_sign[Super_Num] = 1;
	while(*supersonic_echo_addr[Super_Num]==1);
	U_end[Super_Num] = GetEchoTime();
	Supersonic_TIM_Close();
	U_t[Super_Num] = U_end[Super_Num] - U_start[Super_Num];
	U_dis[Super_Num] = U_t[Super_Num]/57.5;//U_dis[j]*0.7 + 0.3*U_t[j]/5.8;
	return 1;
}
// 15 - 2B8
void data_send_pc(u16 dis_buf)
{
	ROS_USB_USART_Send_byte(dis_buf>>8 &0xFF);
	delay_us(30);
	ROS_USB_USART_Send_byte(dis_buf &0xFF);
		
}










