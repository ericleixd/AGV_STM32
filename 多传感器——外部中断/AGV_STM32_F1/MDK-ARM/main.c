  // 相当于51单片机中的  #include <reg51.h>
#include "main.h"
#include <math.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "charge_232_usart.h"
#include "supersonic.h"
#include "infrared.h"
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
	ROS_USB_USART_Init();
	Supersonic_GPIO_Init();
	Supersonic_TIM_Init();
	ADC_GPIO_Config();
	ADC_DMA_COnfig();
	limit_Switch_Init();
	ROS_USB_USART_Send_byte(0xee);

	while(1)
	{
		
		

	}

		
}


