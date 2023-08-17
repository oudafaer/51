#include <reg52.h>

#define uint unsigned int						  //对数据类型进行声明定义
#define uchar unsigned char

sbit LS138_A = P2^2;							  //138芯片A2引脚
sbit LS138_B = P2^3;							  //138芯片A1引脚
sbit LS138_C = P2^4;							  //138芯片A0引脚

uchar DT_s = 0;								  //秒计时
uchar DT_min = 0;									  //分计时

uchar code duan[] = {0x3F,0x06,0x5B,0x4F,		  //共阴数码管无小数点 显示0~9
					 0x66,0x6D,0x7D,0x07,
					 0x7F,0x6F	
					};

uchar code duanpoint[] = {0xbf,0x86,0xdb,0xcf,	   //共阴数码管有小数点 显示0~9
						  0xe6,0xed,0xfd,0x87,
						  0xff,0xef,0xf7,0xfc,
						  0xb9,0xde,0xf9,0xf1
						 };

/************************************************
函数名				：delay1ms
函数功能            ：t=1，大约延时1ms
************************************************/
void delay1ms(uint t)
{
	uint i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<120;j++);	
	}
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

extern unsigned char key_scanf()
{
    unsigned char num_key=0;//按键号
    unsigned char temp=0;//用于读取 P3 线上按键值
    static unsigned char temp_code=0;//保存按键值
    static unsigned char num_check=0;//低电平有效次数
    static unsigned char key_flag=0;//按键有效标识
    temp=P3&0xF0;//读取 P3 线数据
    if(temp!=0xF0)//低电平判断
    {
        num_check++;
        if(num_check==10)//连续 10 次(10ms)低电平有效,则认为按键有效
        {
            key_flag=1;//使能按键有效标识
            temp_code=temp;//保存按键值
		}
		else//松手时判断
		{
			num_check=0;
			if(key_flag==1)//按键有效
			{
				key_flag=0;
				switch(temp_code)//读取按键号
				{
					case 0xE0: num_key=1;
					break;
					case 0xD0: num_key=2;
					break;
					case 0xB0: num_key=3;
					break;
					case 0x70: num_key=4;
					break;
				}
			}
		}
	}
    return (num_key);
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
	LS138_A = 0;			  //秒个位位选
	LS138_B = 0;
	LS138_C = 0;
	P0 = duan[s%10];		  //发送段码，显示秒个位	
	delay1ms(5);			  //间隔一段时间扫描
	
	LS138_A = 1;			  //秒十位位选
	LS138_B = 0;
	LS138_C = 0;
	P0 = duan[s/10];		  //发送段码显示秒十位
	delay1ms(5);			  //间隔一段时间扫描

	LS138_A = 0;			  //分的个位位选
	LS138_B = 1;
	LS138_C = 0;
	P0 = duanpoint[min%10];		  //发送段码，显示分的个位
	delay1ms(5);			  //间隔一段时间扫描
	
	LS138_A = 1;			  //分的十位位选
	LS138_B = 1;			  //间隔一段时间扫描
	LS138_C = 0;
	P0 = duan[min/10];		  //发送段码，显示分的十位
	delay1ms(5);			  //间隔一段时间扫描
	P0 = 0x00;				  //消隐
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
	DisplayKey();
	while(1)
	{
		DigDisplay(DT_s,DT_min);	   //数码管显示函数
		DisplayKey();					   //按键扫描函数
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
