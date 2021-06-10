  // 相当于51单片机中的  #include <reg51.h>
#include "main.h"
#include <math.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "charge_232_usart.h"
#include "supersonic.h"
#include "my_key.h"

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
	
	limit_Key_Init();
	//TIM_Cmd(Supersonic_TIM,ENABLE);
	int t=0;

	while(1)
	{
		delay_ms(10);
		if(limit_Key_Scan()){
			ROS_USB_USART_Send_byte(0xFF);
		}else{
			ROS_USB_USART_Send_byte(0x00);
		}
		

	}

		
}


