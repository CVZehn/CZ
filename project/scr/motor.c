#include "project_config.h"

//八拍驱动方式正转表 A-B- --> B- --> B-A --> A --> AB --> B
// --> BA- --> A- --> A-B-
uchar upstep8_table[]={0x05,0x01,0x09,0x08,0x0A,0x02,0x06,0x04};
 
//四拍驱动方式正转表 A --> B --> A- --> B-
uchar upstep4_table[]={0x08,0x02,0x06,0x01};
 
 
//******************************
//		步进电机停止函数
//作用：停止
//******************************
void MotorStop(void)
{
	P2 = 0x00;
}
 
 
//******************************
//		步进电机驱动函数
//作用：通过变量var控制电动机的转速高低，通过变量state判断电动机的正反转
//			state：0 正转，state: 1 反转
//使用8拍能够实现比较平滑的转动，使用4拍时电机震动比较大。
//******************************
void MotorSpeedOrDirection(uint var, uchar state)
{
	uchar i=0;
	if(!state)
	{
		for(i=0; i<8; i++)
		{
			P2=upstep8_table[i]<<1;
			delay(var);
		}
	}else{
		for(i=7; i>0; --i)
		{
			P2=upstep8_table[i]<<1;
			delay(var);
		}
	}
}
 
 
//******************************
//		步进控制函数
//作用：通过变量var控制旋转圈速
//******************************
void ConStep(uchar var)
{
	uchar i=0,j=0,m=0,k=0;
	while(k++<var)
	{
		for(i=0;i<20;i++)
		{
			for(m=0;m<20;m++)
			{
				P2=upstep8_table[j]<<1;
				j++;
				if(j>=8)             //每8次一个循环
					j=0;
				delay(1);
			}
		}
	}
}

void rise()
{
	uchar i=0;
		for(i=0; i<8; i++)
		{
			P2=upstep8_table[i]<<1;
			delay(1);
		}
}

void decline()
{
    
	uchar i=0;
		for(i=7; i>0; --i)
		{
			P2=upstep8_table[i]<<1;
			delay(1);
		}
}
