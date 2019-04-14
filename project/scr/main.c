#include "project_config.h"
sbit RST=P1^1;
void Delay150us();	//@11.0592MHz

void main()
{
    u8 RXBUFF[4],RXBUFF_L[4];
    u8 TXBUFF[4]={11,11,11,11};
    timer0_Init();
    usart2_init();
    InitADC();
    init_NRF24L01();
	OLED_Init();			//≥ı ºªØOLED  
	OLED_Clear(); 
    OLED_ShowString(36,1,"34",16);
	while(1)
	{
        nRF24L01_TX_Mode(TXBUFF);
        Delay150us();
        nRF24L01_RX_Mode(RXBUFF);
        
        if(RXBUFF[0]!=RXBUFF_L[0])
        {
            if(RXBUFF[0]==open)
            {
                OLED_ShowString(36,3,"112",16);
                *RXBUFF_L=*RXBUFF;
            }
            else if(RXBUFF[0]==close)
            {  
                OLED_ShowString(36,3,"111",16);
                SendString2("”¢");
                *RXBUFF_L=*RXBUFF;
            }
        }
        if(KEY1==0)
        {
            TXBUFF[0]=11;TXBUFF[1]=11;TXBUFF[2]=11;TXBUFF[3]=11;
        }
        else if(KEY2==0)
        {
            TXBUFF[0]=22;TXBUFF[1]=22;TXBUFF[2]=22;TXBUFF[3]=22;
        }
        OLED_ShowNum(5,5,((GetADCResult(0)<<2)|ADC_LOW2),4,16);
        Delay150us();
	}
}
 
void Delay150us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 2;
	j = 153;
	do
	{
		while (--j);
	} while (--i);
}