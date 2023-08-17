#include "reg51.h"		//��Ƭ��ͷ�ļ�
#include "intrins.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

#define OP_READ 0xa1
#define OP_WRITE 0xa0

sbit SDA=P3^4;
sbit SCL=P3^3;

//��������


//��ʱ����
void delay1ms()
{
	u8 i,j;
	for(i=0;i<10;i++)
	for(j=0;j<33;j++);
}
void delaynms(unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		delay1ms();
	}
}
//���ݴ�����������
void start()
{
	SDA=1;  //SDA��ʼ��Ϊ�ߵ�ƽ��1��
	SCL=1;  //��ʼ���ݴ���ʱ��Ҫ��SCLΪ�ߵ�ƽ
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=0;//��ʼ�ź�
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL=0;	//SCLΪ�͵�ƽʱ��SDA�����ݲ�����仯
}
//���ݴ��͹رպ���
void stop()
{
	SDA=0;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=1;
	SCL=0;
}
//AT24C02��ȡ����
unsigned char ReadData()
{
	u8 i,x;
	for(i=0;i<8;i++)
	{
		SCL=1;		//SCl��Ϊ�ߵ�ƽ
		x<<=1;		//��X�еĸ�������λ������λ
		x|=(unsigned char)SDA; //��SDA�ϵ�����ͨ����λ�����������X��
		SCL=0;
	}
	return(x);
}
//AT24C02д�����ݺ���
bit WriteCurrent(unsigned char y)
{
	u8 i;
	bit ack_bit;
	for(i=0;i<8;i++)
	{
		SDA = (bit)(y&0x80);   //ͨ����λ���롱���㽫���λ�����͵�S,��Ϊ����ʱ��λ��ǰ����λ�ں�
		_nop_(); 
		SCL=1;
		_nop_();
		_nop_(); 
		SCL=0;
		y<<=1;
	}
	SDA=1;
	_nop_(); 
	_nop_(); 
	SCL=1;
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ack_bit=SDA;	//�����豸��AT24Cxx)��SDA�͵͵�ƽ
					//��ʾ�Ѿ����յ�һ���ֽ�,���͸ߵ�ƽ����ʾû�н��յ��������쳣
	SCL=0;
	return ack_bit; //����AT24C00Ӧ��λ
}
//ָ����ַд������
void WriteSet(u8 add,u8 dat)
{
	start();	//��ʼ���ݴ���
	WriteCurrent(OP_WRITE);	//ѡ��Ҫ������AT24C02оƬ
	WriteCurrent(add);		//д���ַ
	WriteCurrent(dat);		//��ǰ��ַд������
	stop();					//ֹͣ���ݴ���
	delaynms(4);			//һ���ֽ�д������Ϊ1ms����ʱ
}
//��ǰλ�ö�ȡ����
unsigned char ReadCurrent()
{
	u8 x;
	start();		//��ʼ���ݴ���
	WriteCurrent(OP_READ);
	x=ReadData();		//����ȡ��������X
	stop();
	return x;
}
//ָ����ַ��ȡ����
unsigned char ReadSet(unsigned char set_addr)
{
	start();
	WriteCurrent(OP_WRITE);		//ѡ��Ҫ������ATC02оƬ
	WriteCurrent(set_addr);		//��ָ����ַ�������ݲ��ҷ���
	return (ReadCurrent());
}
//������
void main()
{
	SDA=1;		//SDA=1;SCL=1
	SCL=1;
	WriteSet(0x36,0x01);		//��ָ����ַ��0x36����д�����ݡ�0x01��
	P1=ReadSet(0x36);			//��ָ����ַ����0x36���ж�ȡ���ݲ��ͻ�P1��ʾ
}