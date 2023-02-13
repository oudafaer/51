#include "reg52.h"
#include "lcd1602.h"

#define GPIO_KEY P1

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

unsigned char KeyValue;	//存放读取到的键值
unsigned char KeyState;	//用来存放按键状态
unsigned char Confident[]="You Can ";
unsigned char dat[]="0123456789ABCDEF";

void Delay10ms(); //延时50us
void KeyDowm();	//按键检测函数

void main()
{
	u8 i;
	LcdInit();
	KeyState=0;
	for(i=0;i<8;i++)
	{
		LCDWriteData(Confident[i]);
	}
	while(1)
	{
		KeyDowm();
		if(KeyState)
		{
			KeyState=0;
			LcdWriteCom(0x80+0x40);
			LCDWriteData(dat[KeyValue]);
		}
	}
}

void KeyDowm()
{
	char a;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)
	{
		Delay10ms();
		if(GPIO_KEY!=0x0f)
		{
			KeyState=1;
			GPIO_KEY=0x0f;
			Delay10ms();
			switch(GPIO_KEY)
			{
				case(0x07):KeyValue=0;break;
				case(0x0b):KeyValue=1;break;
				case(0x0d):KeyValue=2;break;
				case(0x0e):KeyValue=3;break;
			}
			GPIO_KEY=0xf0;
			Delay10ms();
			switch(GPIO_KEY)
			{
				case(0x70):KeyValue=KeyValue;break;
				case(0xb0):KeyValue=KeyValue+4;break;
				case(0xd0):KeyValue=KeyValue+8;break;
				case(0xe0):KeyValue=KeyValue+12;break;
			}
	}
		while((a<50)&&(GPIO_KEY!=0xf0))
		{
			Delay10ms();
			a++;
		}
	}
}
	
void Delay10ms()   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}