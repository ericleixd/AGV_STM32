#ifndef __MAIN_TASK_H
#define __MAIN_TASK_H


//״̬��
typedef enum 
{
	PREPARE_STATE=0, // ͨ���ʼ��״̬
	MOVE_STATE = 1,  // �����н�״̬  ҲҪ��ȡ������
	CHARGE_PREPARE_STATE = 2, //���״̬  �ڳ��״̬�п�ʼ����ֹͣ��缸��״̬
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

