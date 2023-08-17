#include <reg52.h>

//函数声明
void Uart_Init(void);
void Uart_Send_Byte(unsigned char c);
void Uart_send_string(unsigned char *dat);
//unsigned char Uart_Receive_Byte();  发送字符串函数
void Uart_action(void);

//LED位定义，八个LED对应八个IO口
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;
sbit led4=P1^4;
sbit led5=P1^5;
sbit led6=P1^6;
sbit led7=P1^7;

//开始接收与停止接收标志位，bit 是位定义
bit Start_Receiving=0,Stop_Receiving=0;

//接收数据缓存数组定义，用于存放从串口接收到的指令数据
unsigned char Buffer[10];   // 空数组

//主函数
void main()
{
	//八个LED初始化设置为高电平（熄灭状态）
	led0 = 1;
	led1 = 1;
	led2 = 1;
	led3 = 1;
	led4 = 1;
	led5 = 1;
	led6 = 1;
	led7 = 1;
	
	//串口相关参数初始化设置
	Uart_Init();
	
	//主循环
	while(1)
	{
		Uart_action();              //进行串口接收数据分析及相应的动作
	}
}

//串口参数初始化设置函数
void Uart_Init(void)
{
    SCON=0x50;                  //串口工作方式1:10位异步收发器，1位起始位，8位数据位，1位停止位;REN=1:允许接收 
//    PCON=0x00;4800 9600                //SMOD=0:波特率不加倍 
    TMOD=0x20;                  //定时器T1工作方式2，8位自动重装载定时器/计数器，用于UART波特率 
    TH1=0xFD;                   //定时器初值设置0xFD，单片机晶振11.0592MHz，对应波特率9600
    TL1=0x00;                   //无需设置该位，保持为0x00
    TR1=1;	                    //开启定时器T1，允许T1计数 
	ES=1;                       //串口中断允许位置1，开启串口中断
    EA=1;                       //总中断允许位，开启总中断
}

//串口发送一个字节函数
void Uart_Send_Byte(unsigned char c)
{
	SBUF = c;                     //将传入的数据赋值给发送缓冲器SBUF，将数据发送出去
	while(!TI);		                //等待数据发送完成，发送完成后发送中断标志位TI被硬件置1，跳出循环
	TI = 0;                       //发送中断标志位TI软件清零，为下次发送数据做准备
}

//串口发送字符串函数
void Uart_send_string(unsigned char *dat)  
{
	while(*dat != '\0')           //判断当前指向数据是否为'\0':字符串结束标志，不相等则执行循环内部程序，相等则跳出循环
	{
		Uart_Send_Byte(*dat);       //发送1个字节数据，1个字符大小为1个字节，8位数据
		dat++;                      //地址加1，指向下一个字符
	}
}

////串口接收一个字节函数
//unsigned char Uart_Receive_Byte()
//{	
//	unsigned char dat;
//	while(!RI);	                //等待数据接收完成，接收完成后接收中断标志位RI被硬件置1，跳出循环
//	RI = 0;                     //接收中断标志位RI软件清零，为接收下一个字节数据做准备
//	dat = SBUF;                 //读取接收缓冲器的值，并赋值给变量dat
//	return (dat);               //函数返回读取值
//}

//串口中断服务程序
void Uart_interrupt(void) interrupt 4 		
{
  static char i=0;              //定义静态变量i，在下次进入此函数时，i的值不变，表示接收数据第i个字节
	unsigned char a=0;            //定义接收数据缓存变量
    if(RI)                      //判断接收中断标志位RI是否为1，为1则表明接收完一个字节，否则表明未接收完成
	{                             
		RI=0;                       //接收中断标志位RI软件清零，为接收下一个字节数据做准备
		a=SBUF;                     //读取接收缓冲器的值，并赋值给变量dat
		                            
		if(a==0xD1)                 //判断是否接收到指令开头第一个字节0xD1，是则开始接收下面的数据，否则不开始接收
		{Start_Receiving=1;}        //开始接收数据标志位置1，表明现在是开始接收数据状态
		                            
		else if(a==0x0D)	          //判断是否接收到指令结束最后一个字节回车0x0D，是则停止接收，否则继续接收
		{                           
			Stop_Receiving=1;         //结束接收数据标志位置1，表明现在是结束接收数据状态
			Start_Receiving=0;        //开始接收数据标志位清0，表明现在已经不再接收数据
			i=0;                      //接收字节数清0，为接收下一条指令做准备
		}                           
                                
		if(Start_Receiving==1)      //判断开始接收数据标志位是否为1，为1则表明是开始接收数据状态，并开始进行存放数据，否则表明不再接收和存放数据
		{                           
			 Buffer[i]=a;             //将接收到的数据存放在接收缓存数组Buffer中
			 i++;                     //接收字节数加1				
		}
		
   }
}

//串口动作函数
void Uart_action(void)
{
	unsigned char reply[]="receive success！\n";
	
  if(Stop_Receiving)            //判断结束接收数据标志位是否为1，为1则表明是结束接收数据状态，并开始进行分析数据，否则表明不再接收和存放数据
	{
		Uart_send_string(reply);    //串口发送字符串"receive success！\n"
    Stop_Receiving=0;           //停止接收标志位清零，为下次分析指令数据做准备
		
		switch(Buffer[1])           //接收缓存数组第2个字节数值开关
		{
			//1.根据接收缓存数组第2个字节的数值进行选择
			//2.根据接收缓存数组第3个字节的数值进行相应的操作，为0则点亮LED，为非0值则熄灭LED
      case 0x01: if(Buffer[2]==0){led0=0;} else{led0=1;} break;
      case 0x02: if(Buffer[2]==0){led1=0;} else{led1=1;} break;
      case 0x03: if(Buffer[2]==0){led2=0;} else{led2=1;} break;
      case 0x04: if(Buffer[2]==0){led3=0;} else{led3=1;} break;
      case 0x05: if(Buffer[2]==0){led4=0;} else{led4=1;} break;
      case 0x06: if(Buffer[2]==0){led5=0;} else{led5=1;} break;
      case 0x07: if(Buffer[2]==0){led6=0;} else{led6=1;} break;
      case 0x08: if(Buffer[2]==0){led7=0;} else{led7=1;} break;
			default: break;
    }
  }
}