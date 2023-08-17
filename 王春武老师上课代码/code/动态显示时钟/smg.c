#include "reg51.h"

sbit a=P1^5;
sbit b=P1^3;
sbit c=P1^1;
sbit Lock=P3^2;
sbit Dat=P3^7;
sbit Clk=P3^6;
sbit SET=P2^0;	 //设置开关
sbit ADD=P2^2;	  //调整参数开关
sbit SUB=P2^4;  


void Send(unsigned char n);	 
void CS(unsigned char n);

char state,hour,minute,second,pos;
unsigned char cnt,buf[8]={0,0,0,0,0,0,2,1},zxm[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};
bit sec,key_mark;

void delay()
{
   char i,j;

   for(i=0;i<4;i++)
     for(j=0;j<100;j++);

}

void T0Ser() interrupt 1
{
   TH0=-5000>>8;
   TL0=-5000;

   if(++cnt==200)
   {
      cnt=0;
	  sec=1;
   }
   
   if(++pos>5)
      pos=0;

   CS(pos);

   if(pos==2||pos==4)
      Send(zxm[buf[pos]] | 1); 
   else
      Send(zxm[buf[pos]]); 
  
}

void CS(unsigned char n)
{
   c=n/4;
   b=n%4/2;
   a=n%2;   
}

void Send(unsigned char n)
{
   char i;

   for(i=0;i<8;i++)
   {
      if(n&1)
	    Dat=1;
	  else
	    Dat=0;
	  
	  Clk=0;
	  Clk=1;
	  n>>=1;		  
   }
   Lock=0;
   Lock=1;
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
}

void main()
{ 
   TMOD=0X01;
   IE=0X82;
   TR0=1;  
   hour=12;
   state=0;

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
	     key_mark=0;

	  if(sec)
	  {
		sec=0;

		if(state==0)
		{
			if(++second>59)
			{
			   second=0;
			   if(+++minute>59)
			   {
			      minute=0;
				  if(++hour>23)
				     hour=0;
			   }			
			}
	    }
		buf[0]=second%10;
		buf[1]=second/10;
		buf[2]=minute%10;
		buf[3]=minute/10;
		buf[4]=hour%10;
		buf[5]=hour/10;
	  }
   }
}