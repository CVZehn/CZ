#ifndef _NRF24L01_H
#define _NRF24L01_H
extern void          _nop_     (void);

#define uchar unsigned char
#define uint unsigned int
//*************************************************************
void init_NRF24L01(void);			   //NRF24L01��ʼ��
void nRF24L01_TX_Mode(uchar * TX_buf); //���� rx_buf������
void nRF24L01_RX_Mode(uchar * RX_buf);  //���ݽ���ģʽ 
#endif

