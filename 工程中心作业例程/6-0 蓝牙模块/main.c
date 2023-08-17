// 51单片机
#include <reg52.h>
#include "UART.h"

typedef unsigned int uint;
typedef unsigned char uchar; 
uint Date=0;      //传递数据位

int main()
{
       UART_init();  //波特率9600
       while(1)
       {
                    if(Date == 0x31)  
					{
						P1=0x00;  //本单片机P2^1为小灯
					}
                    else
					{
                         P1 =0xff; 
					}
       }           
}

void UART_INTERRUPT() interrupt 4
{        
              Date= SBUF;
              RI = 0;   
              SBUF = Date;    //手机和蓝牙模块通讯时，我们可以打开电脑串口调试工具查看我们的发送值）
              while(!TI);
              TI = 0;  
}
