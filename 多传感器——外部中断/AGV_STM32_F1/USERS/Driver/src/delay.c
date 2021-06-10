
#include "delay.h"
static u8 fac_us=0;  //us延时倍乘数
static u16 fac_ms=0;


void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);// 72/8=9
	fac_us=SystemCoreClock/8000000; //系统时钟的1/8 要计数多少次才能是1us
	fac_ms=fac_us*1000;	
}
void delay_us(u32 nus)  //1864135us  24位的计数器 最大值为2^24=
{
	u32 temp;
	SysTick->LOAD=nus*fac_us;
	SysTick->VAL=0x00;
	SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk; //SysTick_CTRL_ENABLE_Msk=1
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&& !(temp&(1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL=0x00;
	
}
void delay_xms(u16 nms) //最大1864
{
	u32 temp;
	SysTick->LOAD=(u32)nms*fac_ms;
	SysTick->VAL=0x00;
	SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk; //SysTick_CTRL_ENABLE_Msk=1
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&& !(temp&(1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL=0x00;
		
}

void delay_ms(u16 nms)
{
    u8 repeat = nms / 540;
    u16 remain = nms % 540;
    while(repeat)
    {
        delay_xms(540);
        repeat--;
    }
    if(remain)
			delay_xms(remain);
}





