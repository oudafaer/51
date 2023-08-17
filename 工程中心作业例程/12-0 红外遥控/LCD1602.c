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
	LcdWriteCom(0x0C);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
}
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		 LcdWriteCom((0x80)|(Column-1));
	}
	else
	{
		LcdWriteCom((0x80)|(Column-1)+0x40);
	}	
}
void LCD_ShowChar(unsigned char Line,unsigned char Column,unsigned char Char)
{
	if(Line==1)
	{
		 LcdWriteCom((0x80)|(Column-1));
	}
	else
	{
		LcdWriteCom((0x80)|(Column-1)+0x40);
	}
	LCDWriteData(Char);
}
void LCD_ShowString(unsigned char Line,unsigned char Column,unsigned char String[])
{
	unsigned char i;
	if(Line==1)
	{
		 LcdWriteCom((0x80)|(Column-1));
	}
	else
	{
		LcdWriteCom((0x80)|(Column-1)+0x40);
	}
	 for(i=0;String[i]!='\0';i++)
	{
		LCDWriteData(String[i]); 
	}
}
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCDWriteData('0'+Number/LCD_Pow(10,i-1)%10);
	}
}
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCDWriteData('+');
		Number1=Number;
	}
	else
	{
		LCDWriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCDWriteData('0'+Number1/LCD_Pow(10,i-1)%10);
	}	
}
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	unsigned char SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCDWriteData('0'+SingleNumber);
		}
		else
		{
			LCDWriteData('A'+SingleNumber-10);
		}
	}	
}
void ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCDWriteData('0'+Number/LCD_Pow(2,i-1)%2);
	}
}