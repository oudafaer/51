#include "reg52.h"  //��Ƭ��ͷ�ļ�

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

//λ����
sbit k1=P3^4;
sbit k2=P3^5;
sbit k3=P3^6;
sbit k4=P3^7;

//��������
void Delay_ms(unsigned int xms);
void Uart_Init(void);
void Uart_Send_Byte(unsigned char c);
void keycontrol(void);

//������
void main()
{
	Uart_Init();
	while(1)
	{
		keycontrol();
	}
}
//��ʱ����
void Delay_ms(unsigned int xms)
{   
	unsigned char i,j;                        
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}
//���ڲ�����ʼ�����ú���
void Uart_Init(void)
{
    SCON=0x50;                  //���ڹ�����ʽ1:10λ�첽�շ�����1λ��ʼλ��8λ����λ��1λֹͣλ;REN=1:������� 
//    PCON=0x00;4800 9600                //SMOD=0:�����ʲ��ӱ� 
    TMOD=0x20;                  //��ʱ��T1������ʽ2��8λ�Զ���װ�ض�ʱ��/������������UART������ 
    TH1=0xFD;                   //��ʱ����ֵ����0xFD����Ƭ������11.0592MHz����Ӧ������9600
    TL1=0x00;                   //�������ø�λ������Ϊ0x00
    TR1=1;	                    //������ʱ��T1������T1���� 
	SM0=0;                      //���ô��ڹ�����ʽ
    SM1=1;
	ES=1;                       //�����ж�����λ��1�����������ж�
    EA=1;                       //���ж�����λ���������ж�
}
//���ڷ���һ���ֽں���
void Uart_Send_Byte(unsigned char c)
{
	SBUF = c;                     //����������ݸ�ֵ�����ͻ�����SBUF�������ݷ��ͳ�ȥ
	while(!TI);		                //�ȴ����ݷ�����ɣ�������ɺ����жϱ�־λTI��Ӳ����1������ѭ��
	TI = 0;                       //�����жϱ�־λTI������㣬Ϊ�´η���������׼��
}
//��������
void keycontrol(void)
{
	unsigned char keynumber=0;
	if(k1==0)
	{
		Delay_ms(10);
		while(k1==0);
		Delay_ms(10);
		keynumber=1;
		Uart_Send_Byte(keynumber);
	}
		if(k2==0)
	{
		Delay_ms(10);
		while(k2==0);
		Delay_ms(10);
		keynumber=2;
		Uart_Send_Byte(keynumber);
	}
	if(k3==0)
	{
		Delay_ms(10);
		while(k3==0);
		Delay_ms(10);
		keynumber=3;
		Uart_Send_Byte(keynumber);
	}
	if(k4==0)
	{
		Delay_ms(10);
		while(k4==0);
		Delay_ms(10);
		keynumber=4;
		Uart_Send_Byte(keynumber);
	}
}