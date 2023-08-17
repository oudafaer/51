#include <reg52.h>
#include "delay.h"
#include "oneWire.h"
#include "DS18B20.h"

unsigned char HANZI[6] = "�¶�Ϊ";   //���庺������
void send(int);                      //���崮�ڷ�����������

//���ô��ڳ�ʼ��
void UsarInit()
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFA; 		//�趨��ʱ��ֵ
	TH1 = 0xFA;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA=1;
	ES=1;
	
}
//������
void main()
{
	UsarInit();
	while(1)
	{
		send(Ds18b20ReadTemp());
		Delay1ms(1000);                     //1s�ӷ���һ��
	}
}

//�¶���������ת���ʹ��ڷ���
void send(int temp) 	 
{
    
  	unsigned char i, wendu[] = {0, 0, 0, 0, 0}; //����洢�¶ȵ�����
	float tp;  	
	tp=temp;                                   //���¶ȸ���һ�������ͱ���
	temp=(tp/16*100)+0.5;	                   //����������ֵ����
	wendu[0] = temp / 10000;                   //�õ���λ
	wendu[1] = temp % 10000 / 1000;            //�õ�ʮλ
	wendu[2] = temp % 1000 / 100;              //�õ���λ
	wendu[3] = temp % 100 / 10;                //�õ�С������һλ
	wendu[4] = temp % 10;                      //�õ�С�����ڶ�λ
	for(i=0; i<6; i++)
	{
	 	SBUF = HANZI[i];//���ͺ���
		while (!TI);			     
		TI = 0;
	}    
	SBUF = '0'+wendu[0];//���͵�һλ����
	while (!TI);			   
	TI = 0;

	SBUF = '0'+wendu[1];//���͵ڶ�λ����
	while (!TI);
	TI = 0;

	SBUF = '0'+wendu[2];//���͵���λ����
	while (!TI);			       
	TI = 0;


	SBUF = '.';//����С����
	while (!TI);			      
	TI = 0;

 
	SBUF = '0'+wendu[3];//����С������һλ����
	while (!TI);			      
	TI = 0;

	SBUF = '0'+wendu[4];//����С�����ڶ�λ����
	while (!TI);			    
	TI = 0; 
}

