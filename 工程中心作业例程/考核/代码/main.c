#include <reg52.h>
#include "delay.h"
#include "oneWire.h"
#include "DS18B20.h"

unsigned char HANZI[6] = "温度为";   //定义汉字数组
void send(int);                      //定义串口发送数据类型

//设置串口初始化
void UsarInit()
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFA; 		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
	
}
//主函数
void main()
{
	UsarInit();
	while(1)
	{
		send(Ds18b20ReadTemp());
		Delay1ms(1000);                     //1s钟发送一次
	}
}

//温度数据类型转换和串口发送
void send(int temp) 	 
{
    
  	unsigned char i, wendu[] = {0, 0, 0, 0, 0}; //定义存储温度的数组
	float tp;  	
	tp=temp;                                   //将温度赋给一个浮点型变量
	temp=(tp/16*100)+0.5;	                   //将浮点型数值扩大
	wendu[0] = temp / 10000;                   //得到百位
	wendu[1] = temp % 10000 / 1000;            //得到十位
	wendu[2] = temp % 1000 / 100;              //得到个位
	wendu[3] = temp % 100 / 10;                //得到小数点后第一位
	wendu[4] = temp % 10;                      //得到小数点后第二位
	for(i=0; i<6; i++)
	{
	 	SBUF = HANZI[i];//发送汉字
		while (!TI);			     
		TI = 0;
	}    
	SBUF = '0'+wendu[0];//发送第一位数字
	while (!TI);			   
	TI = 0;

	SBUF = '0'+wendu[1];//发送第二位数字
	while (!TI);
	TI = 0;

	SBUF = '0'+wendu[2];//发送第三位数字
	while (!TI);			       
	TI = 0;


	SBUF = '.';//发送小数点
	while (!TI);			      
	TI = 0;

 
	SBUF = '0'+wendu[3];//发送小数点后第一位数字
	while (!TI);			      
	TI = 0;

	SBUF = '0'+wendu[4];//发送小数点后第二位数字
	while (!TI);			    
	TI = 0; 
}

