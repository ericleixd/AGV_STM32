#include "my_key.h"

#include "delay.h"
void limit_Key_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(limit_Key_RCC,ENABLE);//使能PORT时钟

	GPIO_InitStructure.GPIO_Pin  = limit_Key_1_GPIO_Pin | limit_Key_2_GPIO_Pin;//KEY1-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(limit_Key_1_GPIO_Port, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin  = limit_Key_3_GPIO_Pin | limit_Key_4_GPIO_Pin;//KEY3-KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(limit_Key_3_GPIO_Port, &GPIO_InitStructure);//初始化

}



//扫描函数
u8 limit_Key_Scan(void)
{	 
	if(limit_Key_1_IN){
		return 1;
	}
    
 	return 0;// 无按键按下
}