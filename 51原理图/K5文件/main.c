#include<reg51.h> //定义头文件
#define uchar unsigned char	  //宏定义
#define uint unsigned int

void delay(uint ms) //定义延时子函数
{
	uint i,j;
	for(i=ms;i>0;i--) //通过改变ms的次数来改变延时时间	 
	  for(j=110;j>0;j--); 
}
	
void main()		 //主函数
{
	while(1)
	{
		P2 = 0Xfe;	  // 1111 1110
		P0 = 0X06;    // 0000 0110 显示1
		delay(500);

		P2 = 0Xfd;	  // 1111 1101
		P0 = 0X5b;    	// 显示2
		delay(500);		

		P2 = 0Xfb;		// 1111 1011
		P0 = 0X4f;    	//显示3
		delay(500);
						   
		P2 = 0Xf7;			// 1111 0111
		P0 = 0X66;    		//显示4
		delay(500);			
		}
}
