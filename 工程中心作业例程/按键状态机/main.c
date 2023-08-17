#include <reg52.h>

#define uint unsigned int						  //���������ͽ�����������
#define uchar unsigned char

sbit LSA=P2^4;
sbit LSB=P2^5;
sbit LSC=P2^6;
sbit LSD=P2^7;

uchar DT_s = 0;								  //���ʱ
uchar DT_min = 0;									  //�ּ�ʱ

/************************************************
������				��delay1ms
��������            ��t=1����Լ��ʱ1ms
************************************************/
void delay(unsigned int i)
{
	while(i--);	
}
void smg(unsigned char x,y)
{
	 unsigned char a[17]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x77,~0x7c,~0x39,~0x5e,~0x79,~0x71};//��ѡ
	 switch(x)
	 {
	  case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;//λѡ
	  case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
	  case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
	  case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	 }
	 P0=a[y];
	 delay(100);
}
void smg1(unsigned char x,y)//ĩβ����С����
{
	 unsigned char a[17]={~0x3f&0x7f,~0x06&0x7f,~0x5b&0x7f,~0x4f&0x7f,~0x66&0x7f,~0x6d&0x7f,~0x7d&0x7f,~0x07&0x7f,~0x7f&0x7f,~0x6f&0x7f,~0x77&0x7f,~0x7c&0x7f,~0x39&0x7f,~0x5e&0x7f,~0x79&0x7f,~0x71&0x7f};//��ѡ�Ҽ���С����
	 switch(x)
	 {
	  case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;//λѡ
	  case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
	  case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
	  case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	 }
	 P0=a[y];
	 delay(100);
}
/************************************************
������				��Timer0Init
��������			:��ʱ/������0�жϳ�ʼ��
*************************************************/
void Timer0Init()
{
	TMOD = 0x01; 		//ѡ��T0��ʱ/�������������ڷ�ʽ1��16λ������		
	TH0 = 0xFC;			//������ʼֵ��������64536��ʼ����1000���������һ�μ�����ʱ��Ϊ1ms
	TL0 = 0x18;		
	ET0 = 1;			//��ʱ/������0�ж�����λ
	EA = 1;				//���ж�
}

/************************************************
������				��S3
��������			�����°�ťS3ʱ��������
*************************************************/
void S3()
{
	DT_s = 0;			//�����
	DT_min = 0;			//�ֹ���
	TR0 = 0;			//���п���λ��0���رն�ʱ��
}
//��ȡ����
uchar key_scanf()
{
   static int Key_on_off = 0 ;//������������
   uchar  num , temp ;
   num = P3 ;            ///��P2��ֵ���ڱ���num��
   num &= 0xf0 ; 	//������λ��0
   if(num != 0xf0)
   {
   	  if(Key_on_off == 0)
	  {
	  	 Key_on_off = 1 ;
		 switch(num)
		 {
		 	//���ذ����ı���
		 	case 0xe0 : temp = 1 ; break ; 
			case 0xd0 : temp = 2 ; break ; 
			case 0xb0 : temp = 3 ; break ; 
			case 0x70 : temp = 4 ; break ; 
		 }
	  }   
   }
   else
   	 Key_on_off = 0 ;
 
	return temp  ;
}
void DisplayKey()
{
	int key;
	key=key_scanf();
	if(key)	 //ȷ�ϰ���������
	{
		switch(key)
		{
			case 1:				  //S1��P3^2��������
						TR0 = 1;	  //��ʱ��0���п���λΪ1��������ʱ��0
						break;
			case 2:				  //S2��P3^1��������
						TR0 = 0;	  //��ʱ��0���п���λΪ0���رն�ʱ��0
						break;
			case 3:				  //S3��P3^3��������
						S3();
						break;
		}
	}
}
/*******************************************************************
������				��DigDisplay
��������			������ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
********************************************************************/
void DigDisplay(uchar s,uchar min)
{
	smg(4,s%10);
	smg(3,s/10);
	smg1(2,min%10);
	smg(1,min/10);
}

/**********************************************************
������				��������
��������			����
**********************************************************/
void main(void)
{
	P0 = 0x00;		  //���˿�ǰд1
	P3 = 0xFF;		  //���˿�ǰд1
	Timer0Init();	  //��ʱ���жϳ�ʼ������
	while(1)
	{
		DisplayKey();
		DigDisplay(DT_s,DT_min);	   //�������ʾ����
	}
}

/**********************************************************
������				��Timer0
��������			����ʱ������
**********************************************************/
void Timer0() interrupt 1
{
	static uint count_s;	
	static uint count_min;		
	TH0 = 0xFC;				//����ֵ��ʼ������64536��ʼ����������ʱΪ65536�����ʱ��Ϊ 1ms
	TL0 = 0x18;
	count_s++;				 //�����
	count_min++;				 //�ּ���
	if(count_s==1000)			 //������1sʱ�����������ʼ����
	{
		count_s = 0;		 //���������
		DT_s++;			 //��ʾ�����ֵ����
		if(DT_s>59)		 //�������Ϊ59
		{
			DT_s = 0;
		}
	}
	if(count_min==60000)		  //������60000msʱ�����������ʼ����
	{
		count_min = 0;		  //�ּ�������
		DT_min++;				  //��ʾ�ּ���ֵ����
		if(DT_min>59)			  //�������Ϊ59
		{
			DT_min = 0;
		}
	}
}
