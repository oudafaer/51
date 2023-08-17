#include "reg52.h"  //����ͷ�ļ�

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

//λ����
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;

//����ȫ�ֱ���
u16 i;

//��������
void Uart_Init();

//���ڲ�����ʼ�����ú���
void Uart_Init(void)
{
    SCON=0x50;                  //���ڹ�����ʽ1:10λ�첽�շ�����1λ��ʼλ��8λ����λ��1λֹͣλ;REN=1:������� 
//    PCON=0x00;4800 9600                //SMOD=0:�����ʲ��ӱ� 
    TMOD=0x20;                  //��ʱ��T1������ʽ2��8λ�Զ���װ�ض�ʱ��/������������UART������ 
    TH1=0xFD;                   //��ʱ����ֵ����0xFD����Ƭ������11.0592MHz����Ӧ������9600
    TL1=0x00;                   //�������ø�λ������Ϊ0x00
    TR1=1;	                    //������ʱ��T1������T1����
    SM0=0;//���ô��ڹ�����ʽ
    SM1=1;
    REN=1;	
	ES=1;                       //�����ж�����λ��1�����������ж�
    EA=1;                       //���ж�����λ���������ж�
}
//������
void main()
{
	Uart_Init();
	while(1);
}
//�жϷ�����
void uart() interrupt 4
{
	u8 receiveData;
	i=SBUF;                   
	 switch(i)
	{
		case 0:	//�յ���0��
			led0=0;
			break;
		
		case 1:	//�յ���1��
			led1=0;
			break;
		
		case 2:	//�յ���2��
			led2=0;
			break;
		
		case 3:	//�յ���3��
			led3=0;
			break;
		default:
			P2=0XFF;
			break;
	}
	receiveData=SBUF;        //��ȥ���յ�������
	RI=0;                    //��������жϱ�־λ
}
