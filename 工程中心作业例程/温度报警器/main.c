#include <reg51.h>
#define uint unsigned int
#define uchar unsigned char           //宏定义
sbit SET = P3^1;                            //定义调整键
sbit DEC = P3^2;                           //定义减少键
sbit ADD = P3^3;                          //定义增加键
sbit BEEP = P3^6;                           //定义蜂鸣器
sbit ALAM = P1^2;                        //定义灯光报警
sbit DQ = P3^7;                             //定义DS18B20总线I/O        
bit shanshuo_st;                            //闪烁间隔标志
bit beep_st;                                     //蜂鸣器间隔标志
sbit DIAN = P0^5;                        //小数点
uchar x=0;                                      //计数器
signed char m;                                     //温度值全局变量
uchar n;                                              //温度值全局变量
uchar set_st=0;                             //状态标志
signed char shangxian=30;                  //上限报警温度
signed char xiaxian=10;                   //下限报警温度
//uchar code  LEDData[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xff};
uchar code  LEDData[]={0x5F,0x44,0x9D,0xD5,0xC6,0xD3,0xDB,0x47,0xDF,0xD7,0xCF,0xDA,0x9B,0xDC,0x9B,0x8B};

//延时子程序
void Delay_DS18B20(int num)
{
  while(num--) ;
}
//初始化DS18B20
void Init_DS18B20(void)
{
  unsigned char x=0;
  DQ = 1;         //DQ复位
  Delay_DS18B20(8);    //稍做延时
  DQ = 0;         //单片机将DQ拉低
  Delay_DS18B20(80);   //精确延时，大于480us
  DQ = 1;         //拉高总线
  Delay_DS18B20(14);
  x = DQ;           //稍做延时后，如果x=0则初始化成功，x=1则初始化失败
  Delay_DS18B20(20);
}
//读一个字节
unsigned char ReadOneChar(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ = 0;     // 给脉冲信号
    dat>>=1;
    DQ = 1;     // 给脉冲信号
    if(DQ)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return 0;
}
//写一个字节
void WriteOneChar(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay_DS18B20(5);
    DQ = 1;
    dat>>=1;
  }
}
//读取温度
unsigned int ReadTemperature(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init_DS18B20();
  WriteOneChar(0xCC);  //跳过读序号列号的操作
  WriteOneChar(0x44);  //启动温度转换
  Init_DS18B20();
  WriteOneChar(0xCC);  //跳过读序号列号的操作
  WriteOneChar(0xBE);  //读取温度寄存器
  a=ReadOneChar();     //读低8位
  b=ReadOneChar();    //读高8位
  t=b;                                   //高8位转移到t
  t<<=8;                           //t数据左移8位
  t=t|a;                           //将t和a按位或，得到一个16位的数
  tt=t*0.0625;                   //将t乘以0.0625得到实际温度值（温度传感器设置12位精度，最小分辨率是0.0625）
  t= tt*10+0.5;     //放大10倍（将小数点后一位显示出来）输出并四舍五入
  return(t);                   //返回温度值
}

//延时子程序
void Delay(uint num)
{
while( --num );
}
//初始化定时器0
void InitTimer(void)
{
        TMOD=0x1;
        TH0=0x3c;
        TL0=0xb0;     //50ms（晶振12M）
}

//读取温度
void check_wendu(void)
{
        uint a,b,c;
        c=ReadTemperature();                          //获取温度值
        a=c/100;                                                     //计算得到十位数字
        b=c/10-a*10;                                            //计算得到个位数字
        m=c/10;                                                      //计算得到整数位
        n=c-a*100-b*10;                                    //计算得到小数位
        if(m<0){m=0;n=0;}                                   //设置温度显示上限
        if(m>99){m=99;n=9;}                                   //设置温度显示上限   
}
//数码管初始化
void Disp_init(void)   
{
        P0 = ~0x80;      //显示----
        P2 = 0x7F;
        Delay(200);
        P2 = 0xDF;
        Delay(200);   
        P2 = 0xF7;
        Delay(200);
        P2 = 0xFD;
        Delay(200);
        P2 = 0xFF;         //关闭显示
}
//显示温度子程序
void Disp_Temperature(void)     //显示温度
{
        P0 = ~0x98;      //显示C
        P2 = 0x7F;
        Delay(100);
        P2=0xff;
        P0=~LEDData[n];    //显示个位
        P2 = 0xDF;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[m%10];    //显示十位
        DIAN = 0;         //显示小数点
        P2 = 0xF7;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[m/10];    //显示百位
        P2 = 0xFD;
        Delay(100);
        P2 = 0xff;         //关闭显示
}
//显示报警温度子程序
void Disp_alarm(uchar baojing)
{
        P0 =~0x98;      //显示C
        P2 = 0x7F;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[baojing%10]; //显示十位
        P2 = 0xDF;
        Delay(100);
        P2=0xff;
        P0 =~LEDData[baojing/10]; //显示百位
        P2 = 0xF7;
        Delay(100);
        P2=0xff;
        if(set_st==1)P0 =~0xCE;
        else if(set_st==2)P0 =~0x1A; //上限H、下限L标示
        P2 = 0xFD;
        Delay(100);
        P2 = 0xff;         //关闭显示
}
//报警子程序
void Alarm()
{
        if(x>=10){beep_st=~beep_st;x=0;}
        if((m>=shangxian&&beep_st==1)||(m<xiaxian&&beep_st==1))
        {
                BEEP = 0;
                ALAM=0;
        }
        else
        {
                BEEP=1;
                ALAM=1;
        }
}
//主函数
void main(void)
{
    uint z;
	InitTimer();    //初始化定时器
	EA=1;      //全局中断开关
	TR0=1;
	ET0=1;      //开启定时器0
	IT0=1;        
	IT1=1;
	check_wendu();
	check_wendu();
	for(z=0;z<300;z++)
	{
	Disp_init();        
	}
	while(1)
	{
	  if(SET==0)
	  {
	   Delay(2000);
	   do{}while(SET==0);
	   set_st++;x=0;shanshuo_st=1;
	   if(set_st>2)set_st=0;
	  }
	  if(set_st==0)
	  {
	   EX0=0;    //关闭外部中断0
	   EX1=0;    //关闭外部中断1
		check_wendu();
		  Disp_Temperature();
	   Alarm();   //报警检测
	  }
	  else if(set_st==1)
	  {
	   BEEP=1;    //关闭蜂鸣器
	   ALAM=1;
	   EX0=1;    //开启外部中断0
	   EX1=1;    //开启外部中断1
	   if(x>=10){shanshuo_st=~shanshuo_st;x=0;}
	   if(shanshuo_st) {Disp_alarm(shangxian);}
	  }
	  else if(set_st==2)
	  {
	   BEEP=1;    //关闭蜂鸣器
	   ALAM=1;
	   EX0=1;    //开启外部中断0
	   EX1=1;    //开启外部中断1
	   if(x>=10){shanshuo_st=~shanshuo_st;x=0;}
	   if(shanshuo_st) {Disp_alarm(xiaxian);}
	  }
	}
}

//定时器0中断服务程序
void timer0(void) interrupt 1
{
	TH0=0x3c;
	TL0=0xb0;
	x++;
}
//外部中断0服务程序
void int0(void) interrupt 0
{

	EX0=0;      //关外部中断0
	if(DEC==0&&set_st==1)
	{	
          do{
                Disp_alarm(shangxian);
        }
         while(DEC==0);
          shangxian--;
          if(shangxian<xiaxian)shangxian=xiaxian;
	}
	else if(DEC==0&&set_st==2)
	{
          do{
                Disp_alarm(xiaxian);
        }
         while(DEC==0);
         xiaxian--;
          if(xiaxian<0)xiaxian=0;
	}
}
//外部中断1服务程序
void int1(void) interrupt 2
{
	EX1=0;      //关外部中断1
	if(ADD==0&&set_st==1)
	{
         do{
                Disp_alarm(shangxian);
        }
         while(ADD==0);
          shangxian++;
          if(shangxian>99)shangxian=99;
	}
	else if(ADD==0&&set_st==2)
	{
          do{
                Disp_alarm(xiaxian);
        }
         while(ADD==0);
          xiaxian++;
          if(xiaxian>shangxian)xiaxian=shangxian;
	}  
}