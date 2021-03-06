#include "project_config.h"

//******************************
//		定时器0初始化函数
//作用：初始化定时器0
//******************************
void timer0_Init()
{
	EA = 1;
	TMOD = 0x11;
	ET0 = 1;
 
	TH0=(65536-1000)/256;
	TL0=(65536-1000)%256;
 
	TR0=1;
}
 
 
//******************************
//		中断处理函数
//作用：定时器0的中断处理
//******************************
void time0int(void) interrupt 1
{
    
//  定时器定时1ms
	TH0=(65536-1000)/256;
	TL0=(65536-1000)%256;
 
	count++;
}
 
 
//******************************
//
//延时i ms
//******************************
void delay(uint ims)
{
	endcount=ims;
	count=0;
	do{}while(count<endcount);
}
 
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
