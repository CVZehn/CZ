#include "reg52.h"
#include "NRF24L01.h"
sbit KEY3=P3^7;
sbit KEY4=P3^6;
sbit KEY5=P3^3;
sbit KEY6=P3^2;
sbit LED1=P1^0;
sbit LED2=P1^1;



void Delay200us()	;	//@11.0592MHz
void Delay50ms();

void main()
{
    unsigned char TXBUFF[]={112,112,112,112};
    unsigned char RXBUFF[4];
    KEY3=1;
    KEY4=1;
    KEY5=1;
    KEY6=1;
    LED1=1;
    init_NRF24L01();
    LED1=0;
    while(1)
    {
        
            TXBUFF[0]=0;TXBUFF[1]=0;TXBUFF[2]=0;TXBUFF[3]=0;
        if(KEY3==0)
        {
                Delay50ms();
            if(KEY3==0)
            {
                TXBUFF[0]=101;TXBUFF[1]=101;TXBUFF[2]=101;TXBUFF[3]=101;
                while(KEY3==0);
            }
        }
        else if(KEY4==0)
        {
                Delay50ms();
             if(KEY4==0)
            {
                TXBUFF[0]=102;TXBUFF[1]=102;TXBUFF[2]=102;TXBUFF[3]=102;
                while(KEY4==0);
            }
        }
        else if(KEY5==0)
        {
                Delay50ms();
            if(KEY5==0)
            {
                TXBUFF[0]=103;TXBUFF[1]=103;TXBUFF[2]=103;TXBUFF[3]=103;
                while(KEY5==0);
            }
        }
        else if(KEY6==0)
        {
            Delay50ms();
            if(KEY6==0)
            {
                TXBUFF[0]=104;TXBUFF[1]=104;TXBUFF[2]=104;TXBUFF[3]=104;
                while(KEY6==0);
            }
        }
        else
        {
            TXBUFF[0]=0;TXBUFF[1]=0;TXBUFF[2]=0;TXBUFF[3]=0;
        }
        if(RXBUFF[0]==00)//桩已经收下状态
        {
            LED1=1;
            LED2=0;
        }
        else if(RXBUFF[0]==11)//桩已经伸出状态
        {
            LED1=0;
            LED2=1;
        }
        else if(RXBUFF[0]==22)//桩正在收入/伸出
        {
            LED1=0;
            LED2=0;
        }
        else if(RXBUFF[0]==33)//桩卡住
        {
            LED1=1;
            LED2=1;
        }
        
        nRF24L01_TX_Mode(TXBUFF);Delay200us();
        nRF24L01_RX_Mode(RXBUFF);Delay200us();
    }
}

void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay200us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 3;
	j = 35;
	do
	{
		while (--j);
	} while (--i);
}
