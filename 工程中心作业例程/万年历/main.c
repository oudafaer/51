/**************************************************************************************
*项目：51单片机时钟万年历设计
*作者：化作尘
*版本：V1.1
*邮箱：2809786963@qq.com
*时间：2020年12月1日16:43:51	
*哔哩哔哩视频地址:https://www.bilibili.com/video/BV1VQ4y1M79K
*注意事项：闹钟根据实物设计，不能仿真，使用的是内部eeprom
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "ds1302.h"
#include "temp.h"
#include "lcd.h"
#include "eeprom.h"

sbit k1 = P1^0;	  		//按键
sbit k2 = P1^1;			 
sbit k3 = P1^2;
sbit k4 = P1^3;

sbit lcdled = P2^4;	 		 //lcd背光
sbit beep = P1^4;			//蜂鸣器

unsigned int ti=0,alarm=0;       //修改第几个时间参数 、修改第几个闹钟参数
unsigned char alarm_hour=0x12,alarm_min=0x00;	 //闹钟时、分参数

enum Mode				//定义枚举、三种模式
{
	DISPLAYDATA,MODIFYDATA,SETALARMCLOCK,NONE,ALARMCLOCK
}mode;

enum Alarmswitch	 //定义闹钟开关
{
	OFF,ON
}alarmswitch;

/*********延时函数***********/		  
void delay(unsigned int t)	 //短延时
{
 	while(t--);
}
void delay_ms(unsigned int t)	//毫秒延时
{
	unsigned int a,b;
	for(a=0;a<t;a++)
	for(b=0;b<120;b++);
}

/********显示日期、时间、星期***********/
void display_data(void)
{
	LcdWriteCom(0x80);
	LcdWritestr("20");	
	LcdWriteData(TIME[6]/16+0x30);	  //年
	LcdWriteData(TIME[6]%16+0x30);
	LcdWriteData('-');
	LcdWriteData(TIME[4]/16+0x30);	  //月
	LcdWriteData(TIME[4]%16+0x30);
	LcdWriteData('-');
	LcdWriteData(TIME[3]/16+0x30);	  //日
	LcdWriteData(TIME[3]%16+0x30);

	LcdWritestr("  ");
	switch(TIME[5])					   //显示星期
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
	LcdWriteData(TIME[2]/16+0x30);	  //时
	LcdWriteData(TIME[2]%16+0x30);
	LcdWriteData(':');
	LcdWriteData(TIME[1]/16+0x30);	  //分
	LcdWriteData(TIME[1]%16+0x30);
	LcdWriteData(':');
	LcdWriteData(TIME[0]/16+0x30);	  //秒
	LcdWriteData(TIME[0]%16+0x30);
	LcdWritestr(" ");
}

/*********显示温度***********/
void displaytemp(int temp) 	 //显示温度
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
* 函 数 名         : keypros
* 函数功能		   : 按键处理函数，判断按键K1是否按下
*******************************************************************************/
void keypros()		   //初始页面按键检测
{
	if(k1 == 0)			   //切换模式
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k1==0)	 //再次判断按键是否按下
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}
		while(k1 == 0);
	}
	else if(k2 == 0)			  //蜂鸣器测试
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k2==0)	 //再次判断按键是否按下
		{
			beep = !beep;  
		}
		while(k2 == 0);
	}
	else if(k3 == 0)		   //背光灯测试
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k3==0)	 //再次判断按键是否按下
		{
			lcdled = !lcdled;  
		}
		while(k3 == 0);
	}
	else if(k4 == 0)			//背光灯测试
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k4==0)	 //再次判断按键是否按下
		{
			alarmswitch=!alarmswitch;  
		}
		while(k4 == 0);
	}		
}

/*************修改时间************/
void modify(void)
{
	static int time=0;
	time++;
	if(k1 == 0)			//切换模式
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k1==0)	 //再次判断按键是否按下
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}

		while(k1 == 0);
	}
	else if(k2 == 0)		   //选择修改参数
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k2==0)	 //再次判断按键是否按下
		{
			ti++; 
			if(ti == 8)ti=0; 
		}
		while(k2 == 0);
	}
	else if(k3 == 0 ||k4 == 0)
	switch(ti)				   //选择进入修改参数
	{
		case 0:
						if(k4==0 | k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k4==0 | k3 ==0)	 //再次判断按键是否按下
							{
							   TIME[0]=0;
							}
							while(k4 == 0 | k3==0);
						}
						
						break;    //?
		case 1:
						if(k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
						break;	 //日
		case 4:
					
						if(k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
						break;	 //月
		case 5:
						
						if(k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
							{
							   TIME[5]++;if(TIME[5]==7)TIME[5]=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
							{
							   TIME[5]--;if(TIME[5]==0xff)TIME[5]=6;
							}
							while(k4==0);
						}
						break;	 //周
		case 6:
						
						if(k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
						break;	 //年
		case 7: 
						if(k3==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
							{
							   mode=DISPLAYDATA;
							   ti=0;
							}
							while(k3==0);
						}
						if(k4==0)
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
							{
							   	Ds1302Init();		  //时钟初始化
								mode = DISPLAYDATA;    //返回日期
								ti = 0;				//还原初始修改
							}
							while(k4==0);
						}
						break;	 //年
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
	switch(ti)				   //选择进入修改参数
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
void setalarmclock(void)	//设置闹钟模式
{
	static int time=0;
	time++;
	
	if(k1 == 0)			 //切换模式
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k1==0)	 //再次判断按键是否按下
		{
			mode+= 1;if(mode == 3)mode = DISPLAYDATA;
		}

		while(k1 == 0);
	}
	if(k2 == 0)			 //选择闹钟修改参数
	{
		delay(1000);   //消除抖动 一般大约10ms
		if(k2==0)	 //再次判断按键是否按下
		{
			alarm++; 
			if(alarm == 3)alarm=0; 
		}
		while(k2 == 0);
	}
	
	switch(alarm)			//选择进入修改参数
	{
		case 0:
				if(k3 == 0)			  //控制闹钟开
				{
					delay(1000);   //消除抖动 一般大约10ms
					if(k3==0)	 //再次判断按键是否按下
					{
						alarmswitch = ON;
						SectorErase(0x2401);
						byte_write(0x2401,alarmswitch);
					}
					while(k3 == 0);
				}
				if(k4 == 0)			 //控制闹钟关闭
				{
					delay(1000);   //消除抖动 一般大约10ms
					if(k4==0)	 //再次判断按键是否按下
					{
						alarmswitch = OFF;
						SectorErase(0x2401);
						byte_write(0x2401,alarmswitch);
					}
					while(k4 == 0);
				}
				break;
		case 1:	
												
						if(k3==0)		  //控制闹钟时针加
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
						if(k4==0)			  //控制闹钟时针减
						{
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k3 ==0)	 //再次判断按键是否按下
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
							delay(1000);   //消除抖动 一般大约10ms
							if(k4 ==0)	 //再次判断按键是否按下
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
		LcdWriteCom(0x80);			 //显示
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
	switch(alarm)				   //选择进入修改参数
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

/************闹钟模式*****************/
void alarmclock(void)
{
    if(alarmswitch==ON && alarm_hour==TIME[2] && alarm_min==TIME[1])  //闹钟
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
		delay(1000);   //消除抖动 一般大约10ms
		if(k4 ==0)	 //再次判断按键是否按下
		{
		 	alarmswitch=OFF;
		}
		while(k4==0);
	}       
}
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	int ucount=0;
	unsigned char lastSec;
	beep= 0;
	LcdInit();	 		//lcd初始化
	//Ds1302Init();		  //时钟初始化
	Ds18b20Init();		  //温度传感器初始化
	SectorErase(0x2001);
//	byte_write(0x2001,0x08);	   //执行一遍初始化
//	byte_write(0x2201,0x00);
//	byte_write(0x2401,0x00);
	alarm_hour=byte_read(0x2601);
	alarm_min=byte_read(0x2201);
	alarmswitch=byte_read(0x2401);

	while(1)		
	{
		switch(mode)	//模式选择
		{
			case DISPLAYDATA:		//时间显示模式
				Ds1302ReadTime(); //更新时间
				if(TIME[0] !=  lastSec)
				{
					lastSec = TIME[0];
					display_data();		  //显示时间 	 秒分时日月周年
					displaytemp(Ds18b20ReadTemp());//显示温度
					if(alarmswitch==ON && alarm_hour==TIME[2] && alarm_min==TIME[1])  //闹钟
					{
						mode = ALARMCLOCK; 
					}
				}
	   		keypros(); 				//按键检测
				
				break;
			case MODIFYDATA:			  //时间修改模式
				modify();
				break;
			case SETALARMCLOCK:			   //设置闹钟模式
				setalarmclock();
				break;
			case ALARMCLOCK:			   //闹钟模式
				alarmclock();
				break;
		}
		
	}				
}





