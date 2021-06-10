#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H


//状态机
typedef enum 
{
	PREPARE_STATE=0, // 通电初始化状态
	MOVE_STATE = 1,  // 正常行进状态  也要读取电量。
	CHARGE_PREPARE_STATE = 2, //充电状态  在充电状态有开始充电和停止充电几种状态
	CHARGE_START_STATE = 0x03,
	CHARGE_STOP_STATE  = 0x12,
	STOP_STATE,
	
}Control_Board_State;

typedef enum 
{
	PREPARE_MODE=0,
	START_CHAGE_MODE,
	STOP_CHAGE_MODE,
	
}INPUT_MODE;





void data_refresh(void);
void State_Translate(void);
int Handle_usart1(void);

#endif /*__MAIN_TASK_H*/	

