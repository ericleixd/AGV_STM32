#ifndef _CHARGE_232_USART_H
#define _CHARGE_232_USART_H


#include "stm32f10x.h"
#include "ros_usb_usart.h"

#define Bufsize 30



//电池的充电模式
typedef enum 
{
	Constant_I_MODE =0x00,
	Constant_V_MODE =0x01,
	
}Charge_Mode;

extern u8 rData1[Bufsize];
extern u8 rData2[Bufsize];
extern u8 rIndex1;
extern u8 handleflag1;

extern float Uref;
extern float	Iref;
extern Charge_Mode charge_mode;
extern u8 sIndexA;


void usart2_init(void);

void USART2_Send_Byte(u8 Data);
void USART2_Send_Data(u8 *Data);

void Send_SCIA(void);
void Charge_Data_Pack(char type);



#endif




