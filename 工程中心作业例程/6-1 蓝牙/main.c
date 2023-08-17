#include "reg52.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

u8 Date=0;

//��������
void UartInit(void);		//���ڳ�ʼ������

/*���ڳ�ʼ������*/
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA=1;
	ES=1;
}


/*������*/
void main()
{
    UartInit();
    while(1)
	{
		if(Date==0x01)
		{
			P1=0x00;
		}
		if(Date==0x02)
		{
			P1==0x02;
		}
	}
}

/*�жϺ���*/
void Uart() interrupt 4
{
    Date=SBUF;        //ȡ�����ܵ�������
    RI=0;			  //��������жϱ�־λ
	while(!TI);
	TI=0;
    //�յ���������date
}
