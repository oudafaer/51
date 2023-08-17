#include <STC89C5xRC.H>
#include "intrins.h" 
unsigned int shuju;
void delay(unsigned int i)
{
	while(i--);
}
void UartInit(void)		//4800bps@12.000MHz
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xF3;		//�趨��ʱ��ֵ
	TH1 = 0xF3;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA=1;
	ES=1;
}

void UNRT_sendbyte(unsigned char byte)
{
	SBUF=byte;
	while(TI==0);
	TI=0;
}

void UNRT_accept() interrupt 4
{
	if(RI==1)
	{
		shuju=SBUF;
		RI=0;
	}
}
void main()
{
	UartInit();
	while(1)
	{
		delay(100000);
		UNRT_sendbyte(shuju);
		delay(100000);
	}
	
}
