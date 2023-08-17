/**************************************************************************************
*��Ŀ��51��Ƭ��ʱ�����������
*���ߣ�������
*�汾��V1.1
*���䣺2809786963@qq.com
*ʱ�䣺2020��12��1��16:43:51	
*����������Ƶ��ַ:https://www.bilibili.com/video/BV1VQ4y1M79K
*ע��������Ӹ���ʵ����ƣ����ܷ��棬ʹ�õ����ڲ�eeprom
***************************************************************************************/

#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "ds1302.h"
#include "temp.h"
#include "lcd.h"
#include "eeprom.h"

sbit k1 = P1^0;	  		//����
sbit k2 = P1^1;			 
sbit k3 = P1^2;
sbit k4 = P1^3;

sbit lcdled = P2^4;	 		 //lcd����
sbit beep = P1^4;			//������

unsigned int ti=0,alarm=0;       //�޸ĵڼ���ʱ����� ���޸ĵڼ������Ӳ���
unsigned char alarm_hour=0x12,alarm_min=0x00;	 //����ʱ���ֲ���

enum Mode				//����ö�١�����ģʽ
{
	DISPLAYDATA,MODIFYDATA,SETALARMCLOCK,NONE,ALARMCLOCK
}mode;

enum Alarmswitch	 //�������ӿ���
{
	OFF,ON
}alarmswitch;

/*********��ʱ����***********/		  
void delay(unsigned int t)	 //����ʱ
{
 	while(t--);
}
void delay_ms(unsigned int t)	//������ʱ
{
	unsigned int a,b;
	for(a=0;a<t;a++)
	for(b=0;b<120;b++);
}

/********��ʾ���ڡ�ʱ�䡢����***********/
void display_data(void)
{
	LcdWriteCom(0x80);
	LcdWritestr("20");	
	LcdWriteData(TIME[6]/16+0x30);	  //��
	LcdWriteData(TIME[6]%16+0x30);
	LcdWriteData('-');
	LcdWriteData(TIME[4]/16+0x30);	  //��
	LcdWriteData(TIME[4]%16+0x30);
	LcdWriteData('-');
	LcdWriteData(TIME[3]/16+0x30);	  //��
	LcdWriteData(TIME[3]%16+0x30);

	LcdWritestr("  ");
	switch(TIME[5])					   //��ʾ����
	{		
		case 0:LcdWritestr("Mon"); break;
		case 1:LcdWritestr("Tue"); break;
		case 2:LcdWritestr("Wed"); break;
		case 3:LcdWritestr("Thu"); break;
		case 4:LcdWritestr("Fri"); break;
		case 5:LcdWritestr("Sat"); break;
		case 6:LcdWritestr("Sun"); break;
	}
	if(alarmswitch==ON)LcdWriteData('.');
	else LcdWriteData(' ');

	LcdWriteCom(0xC0);
	LcdWriteData(' ');
	LcdWriteData(TIME[2]/16+0x30);	  //ʱ
	LcdWriteData(TIME[2]%16+0x30);
	LcdWriteData(':');
	LcdWriteData(TIME[1]/16+0x30);	  //��
	LcdWriteData(TIME[1]%16+0x30);
	LcdWriteData(':');
	LcdWriteData(TIME[0]/16+0x30);	  //��
	LcdWriteData(TIME[0]%16+0x30);
	LcdWritestr(" ");
}

/*********��ʾ�¶�***********/
void displaytemp(int temp) 	 //��ʾ�¶�
{
   	float tp;  
	static char flag = 1;
	if(temp< 0)			
  	{
		LcdWriteCom(0xca);
		LcdWriteData('-'); 
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
 
  	}
 	else
  	{			
		LcdWriteCom(0xca);
		LcdWriteData('+'); 
		tp=temp;
		temp=tp*0.0625*100+0.5;	
	}
		if(flag)
		{
			flag =0;
			temp = 2600;
		}
	if(temp==8500)
		return ;
	LcdWriteData(temp % 10000 / 1000 + 0x30);
	LcdWriteData(temp % 1000 / 100  + 0x30);
	LcdWriteData('.');
	LcdWriteData(temp % 100 / 10 + 0x30);
	LcdWriteData(temp % 10 + 0x30);
}


/*******************************************************************************
* �� �� ��         : keypros
* ��������		   : �������������жϰ���K1�Ƿ���
*******************************************************************************/
void keypros()		   //��ʼҳ�水�����
{
	if(k1 == 0)			   //�л�ģʽ
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}
		while(k1 == 0);
	}
	else if(k2 == 0)			  //����������
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			beep = !beep;  
		}
		while(k2 == 0);
	}
	else if(k3 == 0)		   //����Ʋ���
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k3==0)	 //�ٴ��жϰ����Ƿ���
		{
			lcdled = !lcdled;  
		}
		while(k3 == 0);
	}
	else if(k4 == 0)			//����Ʋ���
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k4==0)	 //�ٴ��жϰ����Ƿ���
		{
			alarmswitch=!alarmswitch;  
		}
		while(k4 == 0);
	}		
}

/*************�޸�ʱ��************/
void modify(void)
{
	static int time=0;
	time++;
	if(k1 == 0)			//�л�ģʽ
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}

		while(k1 == 0);
	}
	else if(k2 == 0)		   //ѡ���޸Ĳ���
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			ti++; 
			if(ti == 8)ti=0; 
		}
		while(k2 == 0);
	}
	else if(k3 == 0 ||k4 == 0)
	switch(ti)				   //ѡ������޸Ĳ���
	{
		case 0:
						if(k4==0 | k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4==0 | k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[0]=0;
							}
							while(k4 == 0 | k3==0);
						}
						
						break;    //?
		case 1:
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[1]++;
							   if(TIME[1]%16 == 0x0a)
							   {
							   		TIME[1] += 16;
									TIME[1] &= 0xf0;
							   }if(TIME[1]==0x60)TIME[1]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[1]--;
							   if(TIME[1]%16==0x0f && TIME[1]!=0xff)
							   {
									TIME[1] &= 0xf9;
							   }
							   if(TIME[1]==0xff)TIME[1]=0x59;
							}
							while(k4==0);
						}
						break;    //?
		case 2:
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[2]++;
							   if(TIME[2]%16 == 0x0a)
							   {
							   		TIME[2] += 16;
									TIME[2] &= 0xf0;
							   }if(TIME[2]==0x24)TIME[2]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[2]--;
							   if(TIME[2]%16==0x0f && TIME[2]!=0xff)
							   {
									TIME[2] &= 0xf9;
							   }
							   if(TIME[2]==0xff)TIME[2]=0x23;
							}
							while(k4==0);
						}
						break;    //?
		case 3:
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[3]++;
							   if(TIME[3]%16 == 0x0a)
							   {
							   		TIME[3] += 16;
									TIME[3] &= 0xf0;
							   }if(TIME[3]==0x32)TIME[3]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[3]--;
							   if(TIME[3]%16==0x0f && TIME[3]!=0xff)
							   {
									TIME[3] &= 0xf9;
							   }
							   if(TIME[3]==0xff)TIME[3]=0x31;
							}
							while(k4==0);
						}
						break;	 //��
		case 4:
					
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[4]++;
							   if(TIME[4]%16 == 0x0a)
							   {
							   		TIME[4] += 16;
									TIME[4] &= 0xf0;
							   }if(TIME[4]==0x13)TIME[4]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[4]--;
							   if(TIME[4]%16==0x0f && TIME[4]!=0xff)
							   {
									TIME[4] &= 0xf9;
							   }
							   if(TIME[4]==0xff)TIME[4]=0x12;
							}
							while(k4==0);
						}
						break;	 //��
		case 5:
						
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[5]++;if(TIME[5]==7)TIME[5]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[5]--;if(TIME[5]==0xff)TIME[5]=6;
							}
							while(k4==0);
						}
						break;	 //��
		case 6:
						
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[6]++;
							   if(TIME[6]%16 == 0x0a)
							   {
							   		TIME[6] += 16;
									TIME[6] &= 0xf0;
							   }if(TIME[6]==0xa0)TIME[6]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   TIME[6]--;
							   if(TIME[6]%16==0x0f && TIME[6]!=0xff)
							   {
									TIME[6] &= 0xf9;
							   }
							   if(TIME[6]==0xff)TIME[6]=0x99;
							}
							while(k4==0);
						}
						break;	 //��
		case 7: 
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   mode=DISPLAYDATA;
							   ti=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   	Ds1302Init();		  //ʱ�ӳ�ʼ��
								mode = DISPLAYDATA;    //��������
								ti = 0;				//��ԭ��ʼ�޸�
							}
							while(k4==0);
						}
						break;	 //��
	}


	if(time == 200)
	{
		display_data();
		if(ti == 7){
						LcdWriteCom(0xca);
						LcdWritestr(" <- OK");
		}
			
	}
	else if(time == 400)
	switch(ti)				   //ѡ������޸Ĳ���
	{
		case 0: 
						LcdWriteCom(0xc7);
						LcdWritestr("  ");	
		break;
		case 1: 
						
						LcdWriteCom(0xc4);
						LcdWritestr("  ");
		break;
		case 2: 
						LcdWriteCom(0xc1);
						LcdWritestr("  ");
						
			break;
		case 3: 
						LcdWriteCom(0x88);
						LcdWritestr("  ");
			break;
		case 4: 
						LcdWriteCom(0x85);
						LcdWritestr("  ");
			break;
		case 5: 
						LcdWriteCom(0x8c);
						LcdWritestr("   ");
			break;
		case 6: 
						LcdWriteCom(0x80);
						LcdWritestr("    ");
		break;
		case 7: 
						LcdWriteCom(0xca);
						LcdWritestr("      ");
			break;
	}else if(time>400) time=0;
	
	delay_ms(1);
	
}
void setalarmclock(void)	//��������ģʽ
{
	static int time=0;
	time++;
	
	if(k1 == 0)			 //�л�ģʽ
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k1==0)	 //�ٴ��жϰ����Ƿ���
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}

		while(k1 == 0);
	}
	if(k2 == 0)			 //ѡ�������޸Ĳ���
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k2==0)	 //�ٴ��жϰ����Ƿ���
		{
			alarm++; 
			if(alarm == 3)alarm=0; 
		}
		while(k2 == 0);
	}
	
	switch(alarm)			//ѡ������޸Ĳ���
	{
		case 0:
				if(k3 == 0)			  //�������ӿ�
				{
					delay(1000);   //�������� һ���Լ10ms
					if(k3==0)	 //�ٴ��жϰ����Ƿ���
					{
						alarmswitch = ON;
						SectorErase(0x2401);
						byte_write(0x2401,alarmswitch);
					}
					while(k3 == 0);
				}
				if(k4 == 0)			 //�������ӹر�
				{
					delay(1000);   //�������� һ���Լ10ms
					if(k4==0)	 //�ٴ��жϰ����Ƿ���
					{
						alarmswitch = OFF;
						SectorErase(0x2401);
						byte_write(0x2401,alarmswitch);
					}
					while(k4 == 0);
				}
				break;
		case 1:	
												
						if(k3==0)		  //��������ʱ���
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   alarm_hour++;
							   if(alarm_hour%16 == 0x0a)
							   {
							   		alarm_hour += 16;
									alarm_hour &= 0xf0;
							   }if(alarm_hour==0x24)alarm_hour=0;
							    SectorErase(0x2601);
								byte_write(0x2601,alarm_hour);
							}
							while(k3==0);
						}
						if(k4==0)			  //��������ʱ���
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   alarm_hour--;
							   if(alarm_hour%16==0x0f && alarm_hour!=0xff)
							   {
									alarm_hour &= 0xf9;
							   }
							   if(alarm_hour==0xff)alarm_hour=0x23;
							   SectorErase(0x2601);
							   byte_write(0x2601,alarm_hour);
							}
							while(k4==0);
						}
				break;
		case 2:
				
						if(k3==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k3 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   alarm_min++;
							   if(alarm_min%16 == 0x0a)
							   {
							   		alarm_min += 16;
									alarm_min &= 0xf0;
							   }if(alarm_min==0x60)alarm_min=0;
							   SectorErase(0x2201);
							   byte_write(0x2201,alarm_min);
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //�������� һ���Լ10ms
							if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
							{
							   alarm_min--;
							   if(alarm_min%16==0x0f && alarm_min!=0xff)
							   {
									alarm_min &= 0xf9;
							   }
							   if(alarm_min==0xff)alarm_min=0x59;
							   SectorErase(0x2201);
							   byte_write(0x2201,alarm_min);
							}
							while(k4==0);
						}
				break;
	}


	
	if(time == 200)
	{
		alarm_hour=byte_read(0x2601);
		alarm_min=byte_read(0x2201);
		alarmswitch=byte_read(0x2401);
		LcdWriteCom(0x80);			 //��ʾ
		LcdWritestr("alarm clock:    ");
		LcdWriteCom(0xc0); 
		if(alarmswitch == OFF)LcdWritestr("  OFF    ");
		else LcdWritestr("  ON     ");
		LcdWriteCom(0xc9); 
		LcdWriteData(alarm_hour/16+0x30);
		LcdWriteData(alarm_hour%16+0x30);
		LcdWriteData(':'); 
		LcdWriteData(alarm_min/16+0x30);
		LcdWriteData(alarm_min%16+0x30);
		LcdWritestr("    ");
	}	else if(time == 400)
	switch(alarm)				   //ѡ������޸Ĳ���
	{
		case 0: 
				LcdWriteCom(0xc0);
				LcdWritestr("      ");	
		break;
		case 1: 
				LcdWriteCom(0xc9);
				LcdWritestr("  ");
		break;
		case 2: 
				LcdWriteCom(0xcc);
				LcdWritestr("  ");
						
			break;
		
	}else if(time>400) time=0;
	
	
	
	
	delay_ms(1);
}

/************����ģʽ*****************/
void alarmclock(void)
{
    if(alarmswitch==ON && alarm_hour==TIME[2] && alarm_min==TIME[1])  //����
	{			
		beep=1;
		delay_ms(100);
		beep=0;
		delay_ms(100);  
		beep=1;
		delay_ms(100);
		beep=0;
		LcdWriteCom(0x80);
		LcdWritestr("   time out!    ");
		LcdWriteCom(0xc0);
		LcdWritestr("now time: ");
		LcdWriteData(alarm_hour/16+0x30);
		LcdWriteData(alarm_hour%16+0x30);
		LcdWriteData(':'); 
		LcdWriteData(alarm_min/16+0x30);
		LcdWriteData(alarm_min%16+0x30);
		LcdWritestr("    ");
		delay_ms(500);
		LcdClean();	
	}
	else mode=DISPLAYDATA;
	if(k4==0)
	{
		delay(1000);   //�������� һ���Լ10ms
		if(k4 ==0)	 //�ٴ��жϰ����Ƿ���
		{
		 	alarmswitch=OFF;
		}
		while(k4==0);
	}       
}
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void main(void)
{
	int ucount=0;
	unsigned char lastSec;
	beep= 0;
	LcdInit();	 		//lcd��ʼ��
	//Ds1302Init();		  //ʱ�ӳ�ʼ��
	Ds18b20Init();		  //�¶ȴ�������ʼ��
	SectorErase(0x2001);
//	byte_write(0x2001,0x08);	   //ִ��һ���ʼ��
//	byte_write(0x2201,0x00);
//	byte_write(0x2401,0x00);
	alarm_hour=byte_read(0x2601);
	alarm_min=byte_read(0x2201);
	alarmswitch=byte_read(0x2401);

	while(1)		
	{
		switch(mode)	//ģʽѡ��
		{
			case DISPLAYDATA:		//ʱ����ʾģʽ
				Ds1302ReadTime(); //����ʱ��
				if(TIME[0] !=  lastSec)
				{
					lastSec = TIME[0];
					display_data();		  //��ʾʱ�� 	 ���ʱ��������
					displaytemp(Ds18b20ReadTemp());//��ʾ�¶�
					if(alarmswitch==ON && alarm_hour==TIME[2] && alarm_min==TIME[1])  //����
					{
						mode = ALARMCLOCK; 
					}
				}
	   		keypros(); 				//�������
				
				break;
			case MODIFYDATA:			  //ʱ���޸�ģʽ
				modify();
				break;
			case SETALARMCLOCK:			   //��������ģʽ
				setalarmclock();
				break;
			case ALARMCLOCK:			   //����ģʽ
				alarmclock();
				break;
		}
		
	}				
}





