  // 相当于51单片机中的  #include <reg51.h>
#include "main.h"
#include <math.h>
#include "led.h"
#include "key.h"
#include "delay.h"
#include "infrared.h"

#include "stm32f10x.h"
//#include "usart.h"
//#include "adc.h"



extern uint16_t result[2];
float voltage[6];

int main(void)
{
	uint8_t n;
	float temp = 0;
	float templ = 0;
	ROS_USB_USART_Init();
	SystemInit();
	delay_init();
	delay_us(100000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	ADC_GPIO_Config();
	ADC_DMA_COnfig();
	while(1)
	{
		ROS_USB_USART_Send_byte(0xff);
		for(n=0;n<2;n++)
		{
		    /* ??????? */
			voltage[n]=(float) result[n]/4096*3.3;
			if(n == 0){//21
				// 20 - 150
				temp = voltage[n];
				//temp = 147737/(temp * 10);
				//temp = pow(temp,1.2134);
				//data_IR_dis_pc((u16)temp);
				if(temp > 2){
					ROS_USB_USART_Send_byte(0x30);
				}else if(temp > 1.55){
					ROS_USB_USART_Send_byte(0x40);
				}else if(temp > 1.25){
					ROS_USB_USART_Send_byte(0x50);
				}else if(temp > 1.1){
					ROS_USB_USART_Send_byte(0x60);
				}else if(temp > 0.85){
					ROS_USB_USART_Send_byte(0x70);
				}else if(temp > 0.73){
					ROS_USB_USART_Send_byte(0x90);
				}
			}

		}
		ROS_USB_USART_Send_byte(0xee);
		delay_us(40000000);
	}
}
