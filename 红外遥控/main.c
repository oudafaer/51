#include "IrReceive.h"
#include "lcd1602.h"

void main(void)
{
	uchar buf[16];
	irReceiveInit();
	lcd1602Init();	//LCD��ʼ��
	lcd1602Clear();	//����
	
	while(1)
	{
		if(IrValue.ok==1)//���ս������
		{
			sprintf(buf,"%d",IrValue.cmd); //�����
			lcd1602WriteString(0,0,buf); //��ӡ�ַ���
			IrValue.ok=0;//ÿ����һ����ʾһ�Σ�
		}
	}
}
