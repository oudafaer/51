#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char           //�궨��
sbit SET = P3^1;                            //���������
sbit DEC = P3^2;                           //������ټ�
sbit ADD = P3^3;                          //�������Ӽ�
sbit BEEP = P3^6;                           //���������
sbit ALAM = P1^2;                        //����ƹⱨ��
sbit DQ = P3^7;                             //����DS18B20����I/O        
bit shanshuo_st;                            //��˸�����־
bit beep_st;                                     //�����������־
sbit DIAN = P0^5;                        //С����
uchar x=0;                                      //������
signed char m;                                     //�¶�ֵȫ�ֱ���
uchar n;                                              //�¶�ֵȫ�ֱ���
uchar set_st=0;                             //״̬��־
signed char shangxian=30;                  //���ޱ����¶�
signed char xiaxian=10;                   //���ޱ����¶�
//uchar code  LEDData[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff};
uchar code  LEDData[]={0x5F,0x44,0x9D,0xD5,0xC6,0xD3,0xDB,0x47,0xDF,0xD7,0xCF,0xDA,0x9B,0xDC,0x9B,0x8B};

//��ʱ�ӳ���
void Delay_DS18B20(int num)
{
  while(num--) ;
}
//��ʼ��DS18B20
void Init_DS18B20(void)
{
  unsigned char x=0;
  DQ = 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ = 1;         //��������
  Delay_DS18B20(14);
  x = DQ;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}
//��һ���ֽ�
unsigned char ReadOneChar(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ = 0;     // �������ź�
    dat>>=1;
    DQ = 1;     // �������ź�
    if(DQ)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return 0;
}
//дһ���ֽ�
void WriteOneChar(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay_DS18B20(5);
    DQ = 1;
    dat>>=1;
  }
}
//��ȡ�¶�
unsigned int ReadTemperature(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0x44);  //�����¶�ת��
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar();     //����8λ
  b=ReadOneChar();    //����8λ
  t=b;                                   //��8λת�Ƶ�t
  t<<=8;                           //t��������8λ
  t=t|a;                           //��t��a��λ�򣬵õ�һ��16λ����
  tt=t*0.0625;                   //��t����0.0625�õ�ʵ���¶�ֵ���¶ȴ���������12λ���ȣ���С�ֱ�����0.0625��
  t= tt*10+0.5;     //�Ŵ�10������С�����һλ��ʾ�������������������
  return(t);                   //�����¶�ֵ
}

//��ʱ�ӳ���
void Delay(uint num)
{
while( --num );
}
//��ʼ����ʱ��0
void InitTimer(void)
{
        TMOD=0x1;
        TH0=0x3c;
        TL0=0xb0;     //50ms������12M��
}

//��ȡ�¶�
void check_wendu(void)
{
        uint a,b,c;
        c=ReadTemperature();                          //��ȡ�¶�ֵ
        a=c/100;                                                     //����õ�ʮλ����
        b=c/10-a*10;                                            //����õ���λ����
        m=c/10;                                                      //����õ�����λ
        n=c-a*100-b*10;                                    //����õ�С��λ
        if(m<0){m=0;n=0;}                                   //�����¶���ʾ����
        if(m>99){m=99;n=9;}                                   //�����¶���ʾ����   
}
//����ܳ�ʼ��
void Disp_init(void)   
{
        P0 = ~0x80;      //��ʾ----
        P2 = 0x7F;
        Delay(200);
        P2 = 0xDF;
        Delay(200);   
        P2 = 0xF7;
        Delay(200);
        P2 = 0xFD;
        Delay(200);
        P2 = 0xFF;         //�ر���ʾ
}
//��ʾ�¶��ӳ���
void Disp_Temperature(void)     //��ʾ�¶�
{
        P0 = ~0x98;      //��ʾC
        P2 = 0x7F;
        Delay(100);
        P2=0xff;
        P0=~LEDData[n];    //��ʾ��λ
        P2 = 0xDF;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[m%10];    //��ʾʮλ
        DIAN = 0;         //��ʾС����
        P2 = 0xF7;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[m/10];    //��ʾ��λ
        P2 = 0xFD;
        Delay(100);
        P2 = 0xff;         //�ر���ʾ
}
//��ʾ�����¶��ӳ���
void Disp_alarm(uchar baojing)
{
        P0 =~0x98;      //��ʾC
        P2 = 0x7F;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[baojing%10]; //��ʾʮλ
        P2 = 0xDF;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[baojing/10]; //��ʾ��λ
        P2 = 0xF7;
        Delay(100);
        P2=0xff;
        if(set_st==1)P0 =~0xCE;
        else if(set_st==2)P0 =~0x1A; //����H������L��ʾ
        P2 = 0xFD;
        Delay(100);
        P2 = 0xff;         //�ر���ʾ
}
//�����ӳ���
void Alarm()
{
        if(x>=10){beep_st=~beep_st;x=0;}
        if((m>=shangxian&&beep_st==1)||(m<xiaxian&&beep_st==1))
        {
                BEEP = 0;
                ALAM=0;
        }
        else
        {
                BEEP=1;
                ALAM=1;
        }
}
//������
void main(void)
{
    uint z;
	InitTimer();    //��ʼ����ʱ��
	EA=1;      //ȫ���жϿ���
	TR0=1;
	ET0=1;      //������ʱ��0
	IT0=1;        
	IT1=1;
	check_wendu();
	check_wendu();
	for(z=0;z<300;z++)
	{
	Disp_init();        
	}
	while(1)
	{
	  if(SET==0)
	  {
	   Delay(2000);
	   do{}while(SET==0);
	   set_st++;x=0;shanshuo_st=1;
	   if(set_st>2)set_st=0;
	  }
	  if(set_st==0)
	  {
	   EX0=0;    //�ر��ⲿ�ж�0
	   EX1=0;    //�ر��ⲿ�ж�1
		check_wendu();
		  Disp_Temperature();
	   Alarm();   //�������
	  }
	  else if(set_st==1)
	  {
	   BEEP=1;    //�رշ�����
	   ALAM=1;
	   EX0=1;    //�����ⲿ�ж�0
	   EX1=1;    //�����ⲿ�ж�1
	   if(x>=10){shanshuo_st=~shanshuo_st;x=0;}
	   if(shanshuo_st) {Disp_alarm(shangxian);}
	  }
	  else if(set_st==2)
	  {
	   BEEP=1;    //�رշ�����
	   ALAM=1;
	   EX0=1;    //�����ⲿ�ж�0
	   EX1=1;    //�����ⲿ�ж�1
	   if(x>=10){shanshuo_st=~shanshuo_st;x=0;}
	   if(shanshuo_st) {Disp_alarm(xiaxian);}
	  }
	}
}

//��ʱ��0�жϷ������
void timer0(void) interrupt 1
{
	TH0=0x3c;
	TL0=0xb0;
	x++;
}
//�ⲿ�ж�0�������
void int0(void) interrupt 0
{

	EX0=0;      //���ⲿ�ж�0
	if(DEC==0&&set_st==1)
	{	
          do{
                Disp_alarm(shangxian);
        }
         while(DEC==0);
          shangxian--;
          if(shangxian<xiaxian)shangxian=xiaxian;
	}
	else if(DEC==0&&set_st==2)
	{
          do{
                Disp_alarm(xiaxian);
        }
         while(DEC==0);
         xiaxian--;
          if(xiaxian<0)xiaxian=0;
	}
}
//�ⲿ�ж�1�������
void int1(void) interrupt 2
{
	EX1=0;      //���ⲿ�ж�1
	if(ADD==0&&set_st==1)
	{
         do{
                Disp_alarm(shangxian);
        }
         while(ADD==0);
          shangxian++;
          if(shangxian>99)shangxian=99;
	}
	else if(ADD==0&&set_st==2)
	{
          do{
                Disp_alarm(xiaxian);
        }
         while(ADD==0);
          xiaxian++;
          if(xiaxian>shangxian)xiaxian=shangxian;
	}  
}