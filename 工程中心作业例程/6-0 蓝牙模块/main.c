// 51��Ƭ��
#include <reg52.h>
#include "UART.h"

typedef unsigned int uint;
typedef unsigned char uchar; 
uint Date=0;      //��������λ

int main()
{
       UART_init();  //������9600
       while(1)
       {
                    if(Date == 0x31)  
					{
						P1=0x00;  //����Ƭ��P2^1ΪС��
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
              SBUF = Date;    //�ֻ�������ģ��ͨѶʱ�����ǿ��Դ򿪵��Դ��ڵ��Թ��߲鿴���ǵķ���ֵ��
              while(!TI);
              TI = 0;  
}
