#include "reg52.h"
#include "LCD1602.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

u8 xianshi[]=" for freedom ";

void main()
{
	u8 i;
	LcdInit();			//��ʼ��
	for(i=0;i<13;i++)
	{
		LCDWriteData(xianshi[i]);
	}
	while(1);
}
