#include "reg52.h"

sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_E=P2^7;
#define LCDData P0
#define uint unsigned int
#define uchar unsigned char

void Lcd1602_Delay1ms(uint c)
{
	uchar a,b;
	for(;c>0;c--)
	{
		for(b=199;b>0;b--)
		{
			for(a=1;a>0;a--);
		}
	}
}
void LcdWriteCom(uchar com)
{
	LCD_E=0;
	LCD_RS=0;
	LCD_RW=0;
	
	LCDData=com;
	Lcd1602_Delay1ms(1);
	
	LCD_E=1;
	Lcd1602_Delay1ms(5);
	LCD_E=0;
}
void LCDWriteData(uchar dat)
{
	LCD_E=0;
	LCD_RS=1;
	LCD_RW=0;
	
	LCDData=dat;
	Lcd1602_Delay1ms(1);
	
	LCD_E=1;
	Lcd1602_Delay1ms(5);
	LCD_E=0;
}
void LcdInit()			//初始化
{
 	LcdWriteCom(0x38);  //开显示
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}