#include "reg52.h"
#include "Delay.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;
 
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ

void Nixie(unsigned char Location,Number)
{
	switch(Location)	 //λѡ��ѡ������������
	{
		case(0):
			LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
		case(1):
			LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
		case(2):
			LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
		case(3):
			LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
		case(4):
			LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
		case(5):
			LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
		case(6):
			LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
		case(7):
			LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ		
	}
	P0=smgduan[Number];//���Ͷ���
	delay(1); //���һ��ʱ��ɨ��	
	P0=0x00;//����	
}
