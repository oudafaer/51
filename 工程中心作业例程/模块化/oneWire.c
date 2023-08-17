#include "reg52.h"
sbit oneWire_DQ=P3^7;   //定义DS18B20总线I/O
//单总线的初始化
unsigned char Ds18b20Init()
{
	unsigned char ACK;
	unsigned int i;
	oneWire_DQ=0;			 //将总线拉低
	i=70;	
	while(i--);             //延时642us
	oneWire_DQ=1;			//将总线拉高
	i=0;
	while(oneWire_DQ)	//等待DS18B20拉低总线
	{
		i++;
		if(i>5000)          //等待>5MS
			ACK=1;       //初始化失败	
	}
	ACK=0;               //初始化成功
	return ACK;
}

//写入一个字节
void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i,j;
	for(j=0;j<8;j++)
	{
		oneWire_DQ=0;			//每写入一位数据之前先把总线拉低1us
		i++;
		oneWire_DQ=dat&0x01; //然后写入一个数据，从最低位开始
		i=6;
		while(i--); //延时68us，持续时间最少60us
		oneWire_DQ=1;	//释放总线
		dat>>=1;        //延时
	}
}
//读取一个字节
unsigned char Ds18b20ReadByte()
{
	unsigned char byte,Bit;
	unsigned int i,j;	
	for(j=8;j>0;j--)
	{
		oneWire_DQ=0;//将总线拉低
		i++;
		oneWire_DQ=1;//释放总线
		i++;
		i++;//延时6us等待数据稳定
		Bit=oneWire_DQ;	 //读取数据
		byte=(byte>>1)|(Bit<<7);						  
		i=4;		        //延时48微秒
		while(i--);
	}				
	return byte;
}