#include "reg51.h"		//单片机头文件
#include "intrins.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

#define OP_READ 0xa1
#define OP_WRITE 0xa0

sbit SDA=P3^4;
sbit SCL=P3^3;

//函数声明


//延时函数
void delay1ms()
{
	u8 i,j;
	for(i=0;i<10;i++)
	for(j=0;j<33;j++);
}
void delaynms(unsigned char n)
{
	unsigned char i;
	for(i=0;i<n;i++)
	{
		delay1ms();
	}
}
//数据传输启动函数
void start()
{
	SDA=1;  //SDA初始化为高电平“1”
	SCL=1;  //开始数据传输时，要求SCL为高电平
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=0;//开始信号
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL=0;	//SCL为低电平时，SDA上数据才允许变化
}
//数据传送关闭函数
void stop()
{
	SDA=0;
	SCL=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA=1;
	SCL=0;
}
//AT24C02读取函数
unsigned char ReadData()
{
	u8 i,x;
	for(i=0;i<8;i++)
	{
		SCL=1;		//SCl置为高电平
		x<<=1;		//将X中的各个二进位向左移位
		x|=(unsigned char)SDA; //将SDA上的数据通过按位“或”运算存入X中
		SCL=0;
	}
	return(x);
}
//AT24C02写入数据函数
bit WriteCurrent(unsigned char y)
{
	u8 i;
	bit ack_bit;
	for(i=0;i<8;i++)
	{
		SDA = (bit)(y&0x80);   //通过按位“与”运算将最高位数据送到S,因为传送时高位在前，低位在后
		_nop_(); 
		SCL=1;
		_nop_();
		_nop_(); 
		SCL=0;
		y<<=1;
	}
	SDA=1;
	_nop_(); 
	_nop_(); 
	SCL=1;
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	ack_bit=SDA;	//接受设备（AT24Cxx)向SDA送低电平
					//表示已经接收到一个字节,若送高电平，表示没有接收到，传送异常
	SCL=0;
	return ack_bit; //返回AT24C00应答位
}
//指定地址写入数据
void WriteSet(u8 add,u8 dat)
{
	start();	//开始数据传递
	WriteCurrent(OP_WRITE);	//选择要操作的AT24C02芯片
	WriteCurrent(add);		//写入地址
	WriteCurrent(dat);		//向当前地址写入数据
	stop();					//停止数据传递
	delaynms(4);			//一个字节写入周期为1ms，延时
}
//当前位置读取数据
unsigned char ReadCurrent()
{
	u8 x;
	start();		//开始数据传递
	WriteCurrent(OP_READ);
	x=ReadData();		//将读取的数存入X
	stop();
	return x;
}
//指定地址读取数据
unsigned char ReadSet(unsigned char set_addr)
{
	start();
	WriteCurrent(OP_WRITE);		//选择要操作的ATC02芯片
	WriteCurrent(set_addr);		//从指定地址独处数据并且返回
	return (ReadCurrent());
}
//主函数
void main()
{
	SDA=1;		//SDA=1;SCL=1
	SCL=1;
	WriteSet(0x36,0x01);		//在指定地址“0x36”中写入数据“0x01”
	P1=ReadSet(0x36);			//从指定地址，“0x36”中读取数据并送回P1显示
}