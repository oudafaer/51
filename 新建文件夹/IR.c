#include <reg52.H>
#include "Timer0.h"
#include "Int0.h"

unsigned int IR_Time;
unsigned char IR_State;

unsigned char IR_Data[4];
unsigned char IR_pData;

unsigned char IR_DataFlag;
unsigned char IR_RepeatFlag;
unsigned char IR_Address;
unsigned char IR_Command;

// ����ң�س�ʼ��
void IR_Init(void)
{
	Timer0Init();
	Int0_Init();
}

//����ң�ػ�ȡ�յ�����֡��־λ
unsigned char IR_GetDataFlag(void)
{
	if(IR_DataFlag)
	{
		IR_DataFlag=0;
		return 1;
	}
	return 0;
}

//����ң�ػ�ȡ�յ�����֡��־λ
unsigned char IR_GetRepeatFlag(void)
{
	if(IR_RepeatFlag)
	{
		IR_RepeatFlag=0;
		return 1;
	}
	return 0;
}
//����ң�ػ�ȡ�յ��ĵ�ַ����
unsigned char IR_GetAddress(void)
{
	return IR_Address;
}

//����ң�ػ�ȡ�յ�����������

unsigned char IR_GetCommand(void)
{
	return IR_Command;
}

//�ⲿ�ж�0�жϺ������½��ش���ִ��
void Int0_Routine(void) interrupt 0
{
	if(IR_State==0)				//״̬0������״̬
	{
		Timer0_SetCounter(0);	//��ʱ��������0
		Timer0_Run(1);			//��ʱ������
		IR_State=1;				//��״̬Ϊ1
	}
	else if(IR_State==1)		//״̬1���ȴ�Start�źŻ�Repeat�ź�
	{
		IR_Time=Timer0_GetCounter();	//��ȡ��һ���жϵ��˴��жϵ�ʱ��
		Timer0_SetCounter(0);	//��ʱ��������0
		//�����ʱΪ13.5ms������յ���Start�ź�
		if(IR_Time>12442-500 && IR_Time<12442+500)
		{
			IR_State=2;			//��״̬Ϊ2
		}
		//�����ʱΪ11.25ms������յ���Repeat�ź�
		else if(IR_Time>10368-500 && IR_Time<10368+500)
		{
			IR_RepeatFlag=1;	//���յ�����֡��־λΪ1
			Timer0_Run(0);		//��ʱ��ֹͣ
			IR_State=0;			//��״̬Ϊ0
		}
		else					//���ճ���
		{
			IR_State=1;			//��״̬Ϊ1
		}
	}
	else if(IR_State==2)		//״̬2����������
	{
		IR_Time=Timer0_GetCounter();	//��ȡ��һ���жϵ��˴��жϵ�ʱ��
		Timer0_SetCounter(0);	//��ʱ��������0
		if(IR_Time>1032-500 && IR_Time<1032+500)
		{
			IR_Data[IR_pData/8]&=~(0x01<<(IR_pData%8));	//���ݶ�Ӧλ��0
			IR_pData++;			//����λ��ָ������
		}
		//�����ʱΪ2250us������յ�������1
		else if(IR_Time>2074-500 && IR_Time<2074+500)
		{
			IR_Data[IR_pData/8]|=(0x01<<(IR_pData%8));	//���ݶ�Ӧλ��1
			IR_pData++;			//����λ��ָ������
		}
		else					//���ճ���
		{
			IR_pData=0;			//����λ��ָ����0
			IR_State=1;			//��״̬Ϊ1
		}
		if(IR_pData>=32)		//������յ���32λ����
		{
			IR_pData=0;			//����λ��ָ����0
			if((IR_Data[0]==~IR_Data[1]) && (IR_Data[2]==~IR_Data[3]))	//������֤
			{
				IR_Address=IR_Data[0];	//ת������
				IR_Command=IR_Data[2];
				IR_DataFlag=1;	//���յ�����֡��־λΪ1
			}
			Timer0_Run(0);		//��ʱ��ֹͣ
			IR_State=0;			//��״̬Ϊ0
		}
	}
}