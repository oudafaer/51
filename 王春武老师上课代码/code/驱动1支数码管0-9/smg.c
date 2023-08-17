#include "reg51.h"
 

unsigned char cnt,n,zxm[]={0x40,0xf9,0x24,0x30,0x19,0x12,0x02,0xf8,0x0,0x10};
bit sec;

void T0Ser() interrupt 1
{
   TH0=-50000>>8;
   TL0=-50000;

   if(++cnt==20)
   {
      cnt=0;
	  sec=1;
   } 
}
 

void main()
{ 
   TMOD=0X01;
   IE=0X82;
   TR0=1;  
   while(1)
   {
      if(sec)
	  {
	     sec=0;
		 P2=zxm[n];

		 if(++n>9)
		    n=0;
		  
	  }
   }
}