#include "reg52.h"
#include "LCD1602.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 xianshi[]=" for freedom ";

void main()
{
	u8 i;
	LcdInit();			//初始化
	for(i=0;i<13;i++)
	{
		LCDWriteData(xianshi[i]);
	}
	while(1);
}
