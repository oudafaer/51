#include "reg52.h"

void UART_init()    //设置串行通信 本晶振为11.0592MHZ，其他的就自己算一下应该设置多少
{
      TMOD = 0x20;
      TH1 = 0xfd;
      TL1 = 0xfd;  //波特率9600
      SM0 = 0;  
      SM1 = 1;   // 串口工作方式1 10位异步
      REN = 1;  //串口允许接收
      TR1 = 1;
      EA = 1;
      ES =1 ;  //串口中断
}
/*
void UART_INTERRUPT() interrupt 4
{        
              pass = SBUF;
              RI = 0;   
              SBUF = pass;    //手机和蓝牙模块通讯时，我们可以打开电脑串口调试工具查看我们的发送值）
              while(!TI);
              TI = 0;  
}
*/
