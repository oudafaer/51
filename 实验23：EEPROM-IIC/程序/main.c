/**************************************************************************************
实验现象：下载程序后数码管后4位显示0，按K1保存显示的数据，按K2读取上次保存的数据，
		  按K3显示数据加一，按K4显示数据清零。最大能写入的数据是255
		  
接线说明： (具体接线图可见开发攻略对应实验的“实验现象”章节)
		   1，单片机-->EEPROM模块
		   		P20-->SDA
				P21-->SCL
		   2，单片机-->动态数码管模块
		   		J22-->J6
				P22-->J9(A)
				P23-->J9(B)
				P24-->J9(C)
		   3，单片机-->独立按键模块
		   		P30-->K2
				P31-->K1
				P32-->K3
				P33-->K4	
	
注意事项：																				  
***************************************************************************************/

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "i2c.h"	

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;	 //定义按键端口

unsigned char num=0;
u8 disp[4];
u8 code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}


/*******************************************************************************
* 函数名         :Keypros()
* 函数功能		 :按键处理函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Keypros()
{
	if(k1==0)
	{
		delay(1000);  //消抖处理
		if(k1==0)
		{
			At24c02Write(1,num);   //在地址1内写入数据num
		}
		while(!k1);
	}
	if(k2==0)
	{
		delay(1000);  //消抖处理
		if(k2==0)
		{
			num=At24c02Read(1);	  //读取EEPROM地址1内的数据保存在num中
		}
		while(!k2);
	}
	if(k3==0)
	{
		delay(100);  //消抖处理
		if(k3==0)
		{
			num++;	   //数据加1
			if(num>255)num=0;
		}
		while(!k3);
	}
	if(k4==0)
	{
		delay(1000);  //消抖处理
		if(k4==0)
		{
			num=0;		 //数据清零
		}
		while(!k4);
	}		
}

/*******************************************************************************
* 函数名         :datapros()
* 函数功能		 :数据处理函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void datapros()
{
	disp[0]=smgduan[num/1000];//千位
	disp[1]=smgduan[num%1000/100];//百位
	disp[2]=smgduan[num%1000%100/10];//个位
	disp[3]=smgduan[num%1000%100%10];		
}


/*******************************************************************************
* 函数名         :DigDisplay()
* 函数功能		 :数码管显示函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<4;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位	
		}
		P0=disp[i];//发送数据
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}		
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	while(1)
	{
		Keypros();	 //按键处理函数
		datapros();	 //数据处理函数
		DigDisplay();//数码管显示函数		
	}		
}

