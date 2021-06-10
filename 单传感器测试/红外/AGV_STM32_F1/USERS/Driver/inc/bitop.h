#ifndef __BITOP_H
#define  __BITOP_H

//��һ���Ĵ�����32λ�ֱ��Ӧ��32����ַ�������ʵ�ַ���ɸı�ÿ��λ
#define BIT_ADDR(addr,bitnum)  ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))  //��������һ����ַ��
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))  //�ѵ�ַת��Ϊһ��ָ��,����ȡֵ��*ȡֵ
//volatile unsigned long  *)(addr) addr��һ��ָ����ָ���ױ��λ�õ�ָ��
#define GPIOA_IDR_ADDR                (GPIOA_BASE+0x08)
#define GPIOB_IDR_ADDR                (GPIOB_BASE+0x08)
#define GPIOC_IDR_ADDR                (GPIOC_BASE+0x08)
#define GPIOD_IDR_ADDR                (GPIOD_BASE+0x08)
#define GPIOE_IDR_ADDR                (GPIOE_BASE+0x08)
#define GPIOF_IDR_ADDR                (GPIOF_BASE+0x08)
#define GPIOG_IDR_ADDR                (GPIOG_BASE+0x08)

//�Ĵ���ODR��ַ
#define GPIOA_ODR_ADDR                (GPIOA_BASE+0x0C)
#define GPIOB_ODR_ADDR                (GPIOB_BASE+0x0C)
#define GPIOC_ODR_ADDR                (GPIOC_BASE+0x0C)
#define GPIOD_ODR_ADDR                (GPIOD_BASE+0x0C)
#define GPIOE_ODR_ADDR                (GPIOE_BASE+0x0C)
#define GPIOF_ODR_ADDR                (GPIOF_BASE+0x0C)
#define GPIOG_ODR_ADDR                (GPIOG_BASE+0x0C)



#define PAOUT(n)               MEM_ADDR(BIT_ADDR(GPIOA_ODR_ADDR,n))
#define PBOUT(n)               MEM_ADDR(BIT_ADDR(GPIOB_ODR_ADDR,n))
#define PCOUT(n)               MEM_ADDR(BIT_ADDR(GPIOC_ODR_ADDR,n))
#define PDOUT(n)               MEM_ADDR(BIT_ADDR(GPIOD_ODR_ADDR,n))
#define PEOUT(n)               MEM_ADDR(BIT_ADDR(GPIOE_ODR_ADDR,n))
#define PFOUT(n)               MEM_ADDR(BIT_ADDR(GPIOF_ODR_ADDR,n))
#define PGOUT(n)               MEM_ADDR(BIT_ADDR(GPIOG_ODR_ADDR,n))

/* ����*/
#define PAIN(n)                MEM_ADDR(BIT_ADDR(GPIOA_IDR_ADDR,n))
#define PBIN(n)                MEM_ADDR(BIT_ADDR(GPIOB_IDR_ADDR,n))
#define PCIN(n)                MEM_ADDR(BIT_ADDR(GPIOC_IDR_ADDR,n))
#define PDIN(n)                MEM_ADDR(BIT_ADDR(GPIOD_IDR_ADDR,n))
#define PEIN(n)                MEM_ADDR(BIT_ADDR(GPIOE_IDR_ADDR,n))
#define PFIN(n)                MEM_ADDR(BIT_ADDR(GPIOF_IDR_ADDR,n))
#define PGIN(n)                MEM_ADDR(BIT_ADDR(GPIOG_IDR_ADDR,n))



#endif
