#include "reg52.h"
#include "delay.h"
#include "oneWire.h"
void  Ds18b20()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//����ROM��������		 
	Ds18b20WriteByte(0x44);	    //�¶�ת������
//	Delay1ms(100);	//�ȴ�ת���ɹ������������һֱˢ�ŵĻ����Ͳ��������ʱ��
   
}
//���Ͷ�ȡ�¶�����
void  Ds18b20ReadT()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //����ROM��������
	Ds18b20WriteByte(0xbe);	 //���Ͷ�ȡ�¶�����
}
//��ȡ�¶�
int Ds18b20ReadTemp()
{
	int temp=0;
	unsigned char tmh,tml;
	Ds18b20();			 	//����ת������
	Ds18b20ReadT();			//���Ͷ�ȡ�¶�����
	tml=Ds18b20ReadByte();		//��ȡ�Ͱ�λ�ֽ�
	tmh=Ds18b20ReadByte();		//��ȡ�߰�λ�ֽ�
	temp=tmh;
	temp<<=8;    
	temp|=tml;
	return temp;
}
