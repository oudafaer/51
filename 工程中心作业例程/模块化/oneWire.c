#include "reg52.h"
sbit oneWire_DQ=P3^7;   //����DS18B20����I/O
//�����ߵĳ�ʼ��
unsigned char Ds18b20Init()
{
	unsigned char ACK;
	unsigned int i;
	oneWire_DQ=0;			 //����������
	i=70;	
	while(i--);             //��ʱ642us
	oneWire_DQ=1;			//����������
	i=0;
	while(oneWire_DQ)	//�ȴ�DS18B20��������
	{
		i++;
		if(i>5000)          //�ȴ�>5MS
			ACK=1;       //��ʼ��ʧ��	
	}
	ACK=0;               //��ʼ���ɹ�
	return ACK;
}

//д��һ���ֽ�
void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i,j;
	for(j=0;j<8;j++)
	{
		oneWire_DQ=0;			//ÿд��һλ����֮ǰ�Ȱ���������1us
		i++;
		oneWire_DQ=dat&0x01; //Ȼ��д��һ�����ݣ������λ��ʼ
		i=6;
		while(i--); //��ʱ68us������ʱ������60us
		oneWire_DQ=1;	//�ͷ�����
		dat>>=1;        //��ʱ
	}
}
//��ȡһ���ֽ�
unsigned char Ds18b20ReadByte()
{
	unsigned char byte,Bit;
	unsigned int i,j;	
	for(j=8;j>0;j--)
	{
		oneWire_DQ=0;//����������
		i++;
		oneWire_DQ=1;//�ͷ�����
		i++;
		i++;//��ʱ6us�ȴ������ȶ�
		Bit=oneWire_DQ;	 //��ȡ����
		byte=(byte>>1)|(Bit<<7);						  
		i=4;		        //��ʱ48΢��
		while(i--);
	}				
	return byte;
}