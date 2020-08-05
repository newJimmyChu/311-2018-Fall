#include "float_i2f.h"
float_bits float_i2f(int i) {
        unsigned frac=0x00000000;
	unsigned num = i;
	unsigned sign = 0x00000000;
        unsigned exp=0x00000000;
	unsigned f=0x00000000;
	unsigned rounding=0x00000100;
	int index=0;int mark=0;
 
	if(i==0x00000000)
	{
	 return 0x00000000;
	}
	if(i==-0x80000000)
	{
	 return 0xcf000000;
	}
	if((num>>31)==1)
	{
	sign=1;
	num=~num;
	num=num+1;
	}
	for(index=0;index<31;index++)
	{
	    f=num<<(index);
            f=f>>31;
	    	    
	    if(f==0x00000001&&index!=0)
	    {
	     mark=31-index;
	     break;
	    }
         }
	if(i==0x00000000)
	{
	 sign=0;exp=0;frac=0;
	}
        exp=127+mark;
        
        num=num<<(32-mark);
        f=num<<23>>23;
	frac=num>>9;
	if(f>rounding)
	{
	  if(frac==0x7fffff)
	  {
	   exp=exp+1;
	  }
	frac=frac+0x00000001;
	if(frac==0x800000)
	{frac=0x0;}
	}
	else if(f==rounding)
	{
        	f=frac<<31>>31;
	if(f==0x00000001)
      		{
		frac=frac+0x00000001;
	        if(frac==0x800000)
		{
		exp=exp+0x00000001;
		frac=0x0;
		}	
		}
	}
	return (sign<<31)|(exp<<23)|(frac&0x7fffff);
}
