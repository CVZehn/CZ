#include "reg52.h"
#include "NRF24L01.h"
sbit KEY3=P3^7;
sbit KEY4=P3^6;
sbit KEY5=P3^5;
sbit KEY6=P3^4;
sbit LED1=P1^0;
sbit LED2=P1^1;



void Delay200us()	;	//@11.0592MHz

void main()
{
    unsigned char TXBUFF[]={112,112,112,112};
    unsigned char RXBUFF[4];
    KEY3=1;
    LED1=1;
    init_NRF24L01();
    LED1=0;
    while(1)
    {
        if(KEY3==0)
        {
            TXBUFF[0]=101;TXBUFF[1]=101;TXBUFF[2]=101;TXBUFF[3]=101;
        }
        else if(KEY4==0)
        {
            TXBUFF[0]=102;TXBUFF[1]=102;TXBUFF[2]=102;TXBUFF[3]=102;
        }
        else if(KEY5==0)
        {
            TXBUFF[0]=103;TXBUFF[1]=103;TXBUFF[2]=103;TXBUFF[3]=103;
        }
        else if(KEY6==0)
        {
            TXBUFF[0]=104;TXBUFF[1]=104;TXBUFF[2]=104;TXBUFF[3]=104;
        }
        if(RXBUFF[0]==00)//׮�Ѿ�����״̬
        {
            LED1=1;
            LED2=0;
        }
        else if(RXBUFF[0]==11)//׮�Ѿ����״̬
        {
            LED1=0;
            LED2=1;
        }
        else if(RXBUFF[0]==22)//׮��������/���
        {
            LED1=0;
            LED2=0;
        }
        else if(RXBUFF[0]==33)//׮��ס
        {
            LED1=1;
            LED2=1;
        }
        nRF24L01_TX_Mode(TXBUFF);Delay200us();
        nRF24L01_RX_Mode(RXBUFF);Delay200us();
    }
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
