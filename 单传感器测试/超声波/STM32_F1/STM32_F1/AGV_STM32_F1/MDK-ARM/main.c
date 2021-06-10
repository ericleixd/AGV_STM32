  // 相当于51单片机中的  #include <reg51.h>
#include "main.h"
#include <math.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "charge_232_usart.h"
#include "supersonic.h"

int main(void)
{
	SystemInit();
	LED_Init();
	delay_init();
	delay_us(100000);
	G1LED = 0;
	G2LED = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//MAIN_TASK_TIM_Init();
	ROS_USB_USART_Init();
	//TIM_Cmd(MAIN_TASK_TIM,ENABLE);
	Supersonic_GPIO_Init();
	Supersonic_TIM_Init();
	//TIM_Cmd(Supersonic_TIM,ENABLE);
	//int t=0;
	while(1)
	{
		ROS_USB_USART_Send_byte(0xFF);
			for(int i =1;i<2;i++){
				Supersonic_Work(i+1);
				if(i==1){
					data_send_pc(U_dis[i]);
				}
				
				delay_us(10000);
			}
			
			//u8 flag = Supersonic_Work(1);
			//data_send_pc(U_dis[0]);
			delay_us(1000000);
			
			
		}

		
}


