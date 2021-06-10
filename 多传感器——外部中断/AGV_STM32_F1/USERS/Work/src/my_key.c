#include "my_key.h"

#include "delay.h"
void limit_Switch_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(limit_Key_RCC,ENABLE);//ʹ��PORTʱ��

	GPIO_InitStructure.GPIO_Pin  = limit_Key_1_GPIO_Pin | limit_Key_2_GPIO_Pin;//KEY1-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(limit_Key_1_GPIO_Port, &GPIO_InitStructure);//��ʼ��

}



//ɨ�躯��
u8 limit_Switch_Scan(void)
{	 
	if(limit_Key_1_IN || limit_Key_2_IN){
		return 1;
	}
 	return 0;
}