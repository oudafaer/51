#include "IrReceive.h"
#include "lcd1602.h"

void main(void)
{
	uchar buf[16];
	irReceiveInit();
	lcd1602Init();	//LCD初始化
	lcd1602Clear();	//清屏
	
	while(1)
	{
		if(IrValue.ok==1)//接收解码完成
		{
			sprintf(buf,"%d",IrValue.cmd); //组包；
			lcd1602WriteString(0,0,buf); //打印字符串
			IrValue.ok=0;//每按下一次显示一次；
		}
	}
}
