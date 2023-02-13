#include "IrReceive.h"

void irReceiveInit(void)//��ʼ��
{
  TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
  TH0=0; //����ֵ
  TL0=0; //��ʼ��ֵ
  ET0=1;    //���ж�
  TR0=1;    
	
	IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
	EX0 = 1;   //ʹ���ⲿ�ж�
	EA = 1;    //�����ж�
}

void IrValueParse(u32 recv)//���ݴ�����
{
	uchar temp1;
	uchar temp2;
	temp1=recv;
	temp2=recv>>8;
	IrValue.usercode=temp1;
	if(temp1!=(uchar)(~temp2))
	{
		IrValue.ok=0;
		return;
	}
	temp1=recv>>16;
	temp2=recv>>24;
	IrValue.cmd=temp1;
	if(temp1!=(uchar)(~temp2))
	{
		IrValue.ok=0;
		return;
	}
	IrValue.ok=1;
}

ir_value_t IrValue;

void timer0_isr(void) interrupt 1 //��ʱ��0�жϷ�����
{
	IrValue.irtime++;
}

void ir_ex0_interrupt() interrupt 0 //�ⲿ�ж�0 �½��ش���
{   
		static bit flag_start=0;
		static uchar IrCountBit=0;     //λ������
		if(IrValue.irtime<60&&IrValue.irtime>=40) 
		{
			flag_start=1;������ 9ms+4.5ms ���ݿ�ʼ����
			IrCountBit=0;//��ʼ����
			IrValue.count_repeat=0;//�ط�������
		}
		if(flag_start)
		{
			IrValue.receive>>=1;
			if(IrValue.irtime>6&&IrValue.irtime<10) //�߼�1
			{
				IrValue.receive|=0x80000000;
			}
			else if(IrValue.irtime<513&&IrValue.irtime>=268)
			{
				IrValue.count_repeat++;
			}
			IrCountBit++;
			if(IrCountBit==33)	//�������
			{
				IrValueParse(IrValue.receive);	
			}
		}
		IrValue.irtime=0;//��ն�ʱ������
}
