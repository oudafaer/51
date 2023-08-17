#include "reg52.h"
#include "delay.h"
#include "oneWire.h"
void  Ds18b20()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);		//跳过ROM操作命令		 
	Ds18b20WriteByte(0x44);	    //温度转换命令
//	Delay1ms(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
   
}
//发送读取温度命令
void  Ds18b20ReadT()
{	

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteByte(0xbe);	 //发送读取温度命令
}
//读取温度
int Ds18b20ReadTemp()
{
	int temp=0;
	unsigned char tmh,tml;
	Ds18b20();			 	//发送转换命令
	Ds18b20ReadT();			//发送读取温度命令
	tml=Ds18b20ReadByte();		//读取低八位字节
	tmh=Ds18b20ReadByte();		//读取高八位字节
	temp=tmh;
	temp<<=8;    
	temp|=tml;
	return temp;
}
