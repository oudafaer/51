/*工作流程：状态0时 显示 DATA1的值，并得到SIGNE1*/
/*状态1时 显示 DATA2的值，并得到SIGNE2*/
/*状态2时 显示 DATA3的值，并得到SIGNE3*/
/*功能上可以实现判断计算优先级*/

#include  "reg51.h"

/*P2 口 外接键盘 */


/*进行端口设置*/
/*矩阵按键键值分配*/

sbit   SET=P2^0;	 //设置开关
sbit   ADD=P2^1;	  //调整参数开关
sbit   SUB=P2^2;    

sbit   LOCK=     P3^2;
sbit   CLK=      P3^1;
sbit   DAT=      P3^0;

sbit   red=      P3^7;
sbit   green=    P3^6;
sbit   yellow=   P3^5;
sbit   speaker=  P1^0;

unsigned char cnt,state,t[6]={0};
bit    sec,key_mark,half_sec;

void init_timer0()
{
   	TMOD=0X01;
	TH0=-50000>>8;
	TL0=-50000;
	IE=0X82;
	TR0=1;
}

void timer0() 	interrupt 1
{
   	TH0=0x3c;
	TL0=0xb0;

	if(cnt==10)
	  half_sec=1,green=0,red=1;

	if(++cnt==20)
    {
	    sec=1;
		cnt=0;
		red=0,green=1;
	}	

} 


/*声明用来产生数码管的控制字*/
unsigned char zxm[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0XFD,0XFF,0X61,0XF5,0XC5};
//{0x11,0xd7,0x32,0x92,0xd4,0x98,0x18,0xd3,0x10,0x90,0xfe,0xff,0x38,0x7e,0x1e};
char hour,minute,second;

void delay()
{
   char i,j;

   for(i=0;i<4;i++)
     for(j=0;j<100;j++);

}
void display()
{
   unsigned char i,j;

   LOCK=0;

   for(i=0;i<6;i++)
   {      
      for(j=0;j<8;j++)
	  {
	     if(t[i]&1)
		   DAT=1;
		 else
		   DAT=0;
		 
		 CLK=0;
		 CLK=1;
		 
		 t[i]>>=1;      
	  }
   }
   LOCK=1;


}

void key()
{
   if(!SET)
   {
      if(++state>3)
	    state=0;

   }
   else if(!ADD)
   {
   	  if(state==1)
	   {
		  if(++hour>23)
		    hour=0;
	   }
	  else if(state==2)
	  {
	     if(++minute>59)
		    minute=0;
	  }    
	  else if(state==3)
	  {
	     if(++second>59)
		   second=0;
	  }
	  
	   
   }
   else if(!SUB)
   {
   	  if(state==1)
	   {
		  if(--hour<0)
		    hour=23;
	   }
	  else if(state==2)
	  {
	     if(--minute<0)
		    minute=59;
	  }    
	  else if(state==3)
	  {
	     if(--second<0)
		   second=59;
	  }
   }

  
}

main()
{
   init_timer0();

   hour=12;
   minute=0;
   second=0;
   state=0;
   display();


   while(1)
   {

	  if((!SET || !ADD || !SUB) && !key_mark)
	  {
	    delay();
		if(!SET || !ADD || !SUB) 
		{
		   key_mark=1;
		   key();
		  
		}
	  }
	  else if(SET && ADD && SUB)
	  {
	     key_mark=0;
		 
	  }

      if(sec)
	  {
	     sec=0;

		 if(state==0)
		 {
		    if(++second>59)
		    {
		       second=0;
			   if(++minute>59)
		    	{
			       minute=0;
			       if(++hour>23)
			         hour=0;
			    }
		    }
		 }

		  t[0]=zxm[second%10];
          t[1]=zxm[second/10];
          t[2]=zxm[minute%10];
          t[3]=zxm[minute/10];
          t[4]=zxm[hour%10];
          t[5]=zxm[hour/10];
		  
		  display();
	  }
	  else if(half_sec)
	  {
	     half_sec=0;
		 
		 
		  t[0]=zxm[second%10];
          t[1]=zxm[second/10];
          t[2]=zxm[minute%10];
          t[3]=zxm[minute/10];
          t[4]=zxm[hour%10];
          t[5]=zxm[hour/10];

		 if(state==1)
           t[4]=t[5]=0xff;
         else if(state==2)
           t[2]=t[3]=0xff;
         else if(state==3)
           t[0]=t[1]=0xff;

		 display();

	  }
   } 

}








