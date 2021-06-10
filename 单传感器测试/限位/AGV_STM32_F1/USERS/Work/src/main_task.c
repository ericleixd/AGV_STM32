/**
  ******************************************************************************
  * @file    main_task.c
  * @author  YingXin Wu
  * @version V1.0
  * @date    
  * @brief   main函数中处理任务的函数
	
  ******************************************************************************
  */ 
	
	
#include "main_task_tim.h"
#include "ros_usb_usart.h"
#include "led.h"
#include "charge_232_usart.h"
#include "delay.h"
#include "battery_state_read.h"


Control_Board_State agv_control_state = PREPARE_STATE;
INPUT_MODE Ros_Input_Mode = PREPARE_MODE;
u8 al_send_flag = 0;


void data_refresh(void)
{
	//来自上位机的发送消息为3个字节（只有充电）
	if(ros_rec_flag == 1)
	{
		ans_msg[2] = ros_rdata[2];
		ans_msg[3] = ros_rdata[3];
		ans_msg[4] = ros_rdata[4];
		ROS_USB_USART_Send_msg(ans_msg);
		//前两位都是0x
		if(ros_rdata[0] == 0xAA && ros_rdata[1] == 0xFF)
		{
			//ROS_USB_USART_Send_byte(0XF1);
			if(ros_rdata[2]== 'U')
			{
				charge_mode = Constant_V_MODE;
				Uref = (ros_rdata[3]<<8 | ros_rdata[4])/100.0;
				Ros_Input_Mode = START_CHAGE_MODE;
				
			}
			else if(ros_rdata[2]== 'I')
			{
				charge_mode = Constant_I_MODE;
				Iref = (ros_rdata[3]<<8 | ros_rdata[4])/100.0;
				Ros_Input_Mode = START_CHAGE_MODE;
			}
			else if(ros_rdata[2]== 'N')
			{
				Ros_Input_Mode = STOP_CHAGE_MODE;
			}
			
			
		}
		for(int i =0;i<ros_rdata_len;i++)
			{
				ros_rdata[i]=0;
			}
			ros_rdata_len = 0;
			ros_rec_flag=0;
	}
	
	
}




void State_Translate(void)
{
	switch(agv_control_state)
	{
		case PREPARE_STATE://进行通信那些的初始化,初始化完进入到充电准备状态
			ROS_USB_USART_Init();
			usart2_init();
			BATTERY_STATE_USART_Init();
			BATTERY_STATE_Rx_DMA_Config((u32)battery_data, buffer_size);
			agv_control_state = CHARGE_PREPARE_STATE;
			break;
		case MOVE_STATE://行进状态，时刻刷新超声波和电池电量数据进行监控
			
			break;
		case STOP_STATE: //急停状态
			
			break;
		case CHARGE_PREPARE_STATE://当电量不足时进入充电准备状态，发送消息给工控机，找到充电位置进行充电
			
			if(count ==500)
			{
				G1LED = !G1LED;
				count=0;
			}		
			switch(Ros_Input_Mode)
			{ 
				case START_CHAGE_MODE:
					agv_control_state = CHARGE_START_STATE;
					al_send_flag = 0;
				break;
				case PREPARE_MODE:
				break;
				case STOP_CHAGE_MODE:
					agv_control_state = CHARGE_STOP_STATE;
					Ros_Input_Mode=PREPARE_MODE;
				break;
				default:
					break;	
			}	
			break;
		case CHARGE_START_STATE:  //到达指定位置之后，开始充电（只能从CHARGE_PREPARE_STATE转移
			if(count ==500)
			{
				G2LED = !G2LED;
				count=0;
			}	
			if (al_send_flag==0)
					{
						Charge_Data_Pack(CHARGE_START_STATE);			
						while(sIndexA != 0xFF)
							{
								Send_SCIA();
								delay_ms(1);
							}
							al_send_flag=1;
					}
			switch(Ros_Input_Mode)
			{ 
				case START_CHAGE_MODE:					
				break;
				case PREPARE_MODE:
					agv_control_state=CHARGE_PREPARE_STATE;
				break;
				case STOP_CHAGE_MODE:
					agv_control_state = CHARGE_STOP_STATE;
					Ros_Input_Mode=PREPARE_MODE;
				break;
				default:
					break;	
			}	
			break;
		case CHARGE_STOP_STATE:  //停止充电			
				Charge_Data_Pack(CHARGE_STOP_STATE);			
				while(sIndexA != 0xFF)
					{
						Send_SCIA();
						delay_ms(1);
					}
				agv_control_state=CHARGE_PREPARE_STATE;
			break;
		default:
			break;
	}
	
}








