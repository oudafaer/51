#include "public.h"			 
#include "temp.h"


sbit ESP8266_RST_Pin=P2^5;
sbit ESP8266_CH_PD_Pin=P2^6; 

sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
sbit relay=P2^3;

#define RELOAD_COUNT 0xFA //�궨�岨���ʷ�����������ֵ 9600

#if 1
u8 RecBuf[50];
u8 recFlagOK=0;
#endif

u8 tempData[9];

void UART_Init()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=RELOAD_COUNT;	//��������ʼֵ����
	TL1=TH1;
	ES=0;				//�رս����ж�
	EA=1;				//�����ж�
	TR1=1;				//�򿪼�����
}

void UART_SendByte(u8 dat)
{
	ES=0; //�رմ����ж�
	TI=0; //�巢������ж������־λ
	SBUF=dat; //����
	while(TI==0); //�ȴ��������
	TI=0; //�巢������ж������־λ
	ES=1; //�������ж�
}

void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\r');//�س�
	delay_10us(5);
	UART_SendByte('\n');//����
	delay_ms(1000);
}

void ESP8266_SendData(u8 *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,9");
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendByte(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendByte('\n');//����	
	delay_ms(10);
}

//ESP8266-WIFIģ�鹤��ģʽ��ʼ��
void ESP8266_ModeInit(void)
{
	ESP8266_RST_Pin=1;
	ESP8266_CH_PD_Pin=1;
	ESP8266_SendCmd("AT");
	ESP8266_SendCmd("AT+CWMODE=3");//����·����ģʽ 1 staTIonģʽ 2 AP�� ·����ģʽ 3 station+AP���ģʽ
	ESP8266_SendCmd("AT+CWSAP=\"PRECHIN\",\"prechin168\",11,0"); //����WIFI�ȵ���������
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.2\"");//��������wifiģ��
	ESP8266_SendCmd("AT+RST");//��������wifiģ��
	ESP8266_SendCmd("AT+CIPMUX=1");	//����������ģʽ�����������ͻ��˽���
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//����TCP/IP �˿�Ϊ8080 ʵ�ֻ����������	
}

//�¶����ݴ���
#if 1
void TempDataPros(void)
{ 
	int tempdat=0;

	tempdat=Ds18b20ReadTemp()*10;
	if(tempdat<0)
	{
		tempData[0]='-';
	}		
	else
	{
		tempData[0]=' ';	
	}
			

	tempData[1]=tempdat / 10000+0x30;
	tempData[2]=tempdat % 10000 / 1000+0x30;
	tempData[3]=tempdat % 1000 / 100+0x30;
	tempData[4] ='.';
	tempData[5]=tempdat % 100 / 10+0x30;
	tempData[6]=tempdat % 10+0x30;
	tempData[7] ='C';
	tempData[8] ='\0';		
}	
#endif

void main()
{	
	u16 i=0;

	led1=led2=led3=1;
	relay=1;

	UART_Init(); //���ڳ�ʼ��
	ESP8266_ModeInit();
	ES=1; //�������ж�
	while(1)
	{
		i++;
		if(i%10==0)
		{
			led3=!led3;
			TempDataPros();
			ESP8266_SendData(tempData);
		}
		delay_ms(10);
	}		
}

#if 1
void Uart() interrupt 4
{
	static u8 i=0;
	if(RI)
	{
		RI=0;
		RecBuf[i]=SBUF;	   //+IPD,0,1:F
		if(RecBuf[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			if(RecBuf[0]=='+'&&RecBuf[1]=='I'&&RecBuf[2]=='P'&&RecBuf[3]=='D')
			{
				switch(RecBuf[9])
				{
					case 'F': led1=0;break;//����led1
					case 'B': led1=1;break;//�ر�led1
					case 'L': relay=0;break;//�̵���
					case 'R': relay=1;break;//
				}
			}
		}				
	}
						
}
#endif

