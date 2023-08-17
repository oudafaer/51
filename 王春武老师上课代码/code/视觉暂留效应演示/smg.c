#include "reg51.h"

sbit a=P1^5;
sbit b=P1^3;
sbit c=P1^1;
sbit Lock=P3^2;
sbit Dat=P3^7;
sbit Clk=P3^6;
sbit k1=P3^4;

void Send(unsigned char n);	 
void CS(unsigned char n);

unsigned int n;
unsigned char hour,minute,second,bsecond,cnt,pos,buf[8]={0,0,0,0,0,0,2,1},zxm[]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};
bit sec,keymark;

void T0Ser() interrupt 1
{
   TH0=(65536-n)>>8;
   TL0=62236-n;
   
   if(++pos>5)
      pos=0;
   
   CS(7); 
   Send(zxm[buf[pos]]); 
   CS(pos);
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

void main()
{ 
   TMOD=0X01;
   IE=0X82;
   TR0=1;  
 
   n=0;

   while(1)
   {        
	  if(!k1&&!keymark)
	  {
	     keymark=1;
	  	 n=n-10000;
		 if(n<10000)
		    n=1000;
		 P2=~P2;
	  }
	  else if(k1)
	     keymark=0;
   }
}