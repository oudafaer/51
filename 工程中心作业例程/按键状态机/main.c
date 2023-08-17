#include <reg52.h>

#define uint unsigned int						  //对数据类型进行声明定义
#define uchar unsigned char

sbit LSA=P2^4;
sbit LSB=P2^5;
sbit LSC=P2^6;
sbit LSD=P2^7;

uchar DT_s = 0;								  //秒计时
uchar DT_min = 0;									  //分计时

/************************************************
函数名				：delay1ms
函数功能            ：t=1，大约延时1ms
************************************************/
void delay(unsigned int i)
{
	while(i--);	
}
void smg(unsigned char x,y)
{
	 unsigned char a[17]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x6f,~0x77,~0x7c,~0x39,~0x5e,~0x79,~0x71};//段选
	 switch(x)
	 {
	  case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;//位选
	  case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
	  case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
	  case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	 }
	 P0=a[y];
	 delay(100);
}
void smg1(unsigned char x,y)//末尾加上小数点
{
	 unsigned char a[17]={~0x3f&0x7f,~0x06&0x7f,~0x5b&0x7f,~0x4f&0x7f,~0x66&0x7f,~0x6d&0x7f,~0x7d&0x7f,~0x07&0x7f,~0x7f&0x7f,~0x6f&0x7f,~0x77&0x7f,~0x7c&0x7f,~0x39&0x7f,~0x5e&0x7f,~0x79&0x7f,~0x71&0x7f};//段选且加上小数点
	 switch(x)
	 {
	  case 1:LSA=0;LSB=1;LSC=1;LSD=1;break;//位选
	  case 2:LSA=1;LSB=0;LSC=1;LSD=1;break;
	  case 3:LSA=1;LSB=1;LSC=0;LSD=1;break;
	  case 4:LSA=1;LSB=1;LSC=1;LSD=0;break;
	 }
	 P0=a[y];
	 delay(100);
}
/************************************************
函数名				：Timer0Init
函数功能			:定时/计数器0中断初始化
*************************************************/
void Timer0Init()
{
	TMOD = 0x01; 		//选择T0定时/计数器，工作在方式1，16位计数器		
	TH0 = 0xFC;			//计数初始值，计数从64536开始，计1000个数，完成一次计数，时间为1ms
	TL0 = 0x18;		
	ET0 = 1;			//定时/计数器0中断允许位
	EA = 1;				//总中断
}

/************************************************
函数名				：S3
函数功能			：按下按钮S3时，秒表归零
*************************************************/
void S3()
{
	DT_s = 0;			//秒归零
	DT_min = 0;			//分归零
	TR0 = 0;			//运行控制位清0，关闭定时器
}
//读取按键
uchar key_scanf()
{
   static int Key_on_off = 0 ;//按键自锁变量
   uchar  num , temp ;
   num = P3 ;            ///将P2的值存在变量num中
   num &= 0xf0 ; 	//将低四位清0
   if(num != 0xf0)
   {
   	  if(Key_on_off == 0)
	  {
	  	 Key_on_off = 1 ;
		 switch(num)
		 {
		 	//返回按键的编码
		 	case 0xe0 : temp = 1 ; break ; 
			case 0xd0 : temp = 2 ; break ; 
			case 0xb0 : temp = 3 ; break ; 
			case 0x70 : temp = 4 ; break ; 
		 }
	  }   
   }
   else
   	 Key_on_off = 0 ;
 
	return temp  ;
}
void DisplayKey()
{
	int key;
	key=key_scanf();
	if(key)	 //确认按键被按下
	{
		switch(key)
		{
			case 1:				  //S1（P3^2）被按下
						TR0 = 1;	  //定时器0运行控制位为1，启动定时器0
						break;
			case 2:				  //S2（P3^1）被按下
						TR0 = 0;	  //定时器0运行控制位为0，关闭定时器0
						break;
			case 3:				  //S3（P3^3）被按下
						S3();
						break;
		}
	}
}
/*******************************************************************
函数名				：DigDisplay
函数功能			：数码管动态扫描函数，循环扫描8个数码管显示
********************************************************************/
void DigDisplay(uchar s,uchar min)
{
	smg(4,s%10);
	smg(3,s/10);
	smg1(2,min%10);
	smg(1,min/10);
}

/**********************************************************
函数名				：主函数
函数功能			：无
**********************************************************/
void main(void)
{
	P0 = 0x00;		  //读端口前写1
	P3 = 0xFF;		  //读端口前写1
	Timer0Init();	  //定时器中断初始化函数
	while(1)
	{
		DisplayKey();
		DigDisplay(DT_s,DT_min);	   //数码管显示函数
	}
}

/**********************************************************
函数名				：Timer0
函数功能			：定时器计数
**********************************************************/
void Timer0() interrupt 1
{
	static uint count_s;	
	static uint count_min;		
	TH0 = 0xFC;				//计数值初始化，从64536开始计数，计满时为65536，溢出时即为 1ms
	TL0 = 0x18;
	count_s++;				 //秒计数
	count_min++;				 //分计数
	if(count_s==1000)			 //计数到1s时，秒计数器开始工作
	{
		count_s = 0;		 //秒计数清零
		DT_s++;			 //显示秒计数值自增
		if(DT_s>59)		 //秒数最大为59
		{
			DT_s = 0;
		}
	}
	if(count_min==60000)		  //计数到60000ms时，秒计数器开始工作
	{
		count_min = 0;		  //分计数清零
		DT_min++;				  //显示分计数值自增
		if(DT_min>59)			  //分数最大为59
		{
			DT_min = 0;
		}
	}
}
