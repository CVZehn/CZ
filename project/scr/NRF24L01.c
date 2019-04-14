#include <reg52.h>
#include "NRF24L01.h"
//****************************************NRF24L01�˿ڶ���***************************************
sbit CE   = P1^7;
sbit CSN  = P1^6;
sbit SCK  = P1^5;
sbit MOSI = P1^4;//�޸ĺ�
sbit MISO = P1^3;

//*********************************************NRF24L01*************************************
uchar const TX_ADDRESS[5]={0x00,0x00,0x00,0x00,0xA1};//���ձ��� ��ַ
uchar const RX_ADDRESS[5]={0x00,0x00,0x00,0x00,0xA1};//����Ŀ�� ��ַ
//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define ADR_WIDTH       5   	// 3-5  �ֽ� ��ַ�趨����
#define PLOAD_WIDTH     4    	// 1-32 �ֽ� �����趨����
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ��������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//******************************************************************************************
uchar 	bdata sta;   //״̬��־
sbit	RX_DR	=sta^6;
sbit	TX_DS	=sta^5;
sbit	MAX_RT	=sta^4;
//*****************************************����ʱ*****************************************
void delays(unsigned char s)
{
	unsigned char i;
	for(i=0; i<s; i++)
 {
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
 }
}	
/****************************************************************************************************
/*���ܣ�NRF24L01�� SPIͬ����дʱ��
/****************************************************************************************************/
uchar SPI_RW(uchar reg)
{
	uchar bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
   	{
		MOSI = (reg & 0x80);         // output 'uchar', MSB to MOSI
		reg = (reg << 1);           // shift next bit into MSB..
		SCK = 1;                      // Set SCK high..
		reg |= MISO;       		  // capture current MISO bit
		SCK = 0;            		  // ..then set SCK low again
   	}
    return(reg);           		  // return read uchar
}
/****************************************************************************************************
/*���ܣ�NRF24L01��SPI���Ĵ���ʱ��
/****************************************************************************************************/
uchar SPI_Read(uchar reg)
{
	uchar reg_val;
	CSN = 0;                // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	CSN = 1;                // CSN high, terminate SPI communication
	return(reg_val);        // return register value
}
/****************************************************************************************************/
/*���ܣ�NRF24L01��д�Ĵ�������
/****************************************************************************************************/
uchar SPI_RW_Reg(uchar reg, uchar value)
{
	uchar status;
	CSN = 0;                   // CSN low, init SPI transaction
	status = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	CSN = 1;                   // CSN high again
	return(status);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
/*����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
/****************************************************************************************************/
void SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar i;
	CSN = 0;                    		// Set CSN low, init SPI tranaction
	SPI_RW(reg);       		// Select register to write to and read status uchar
	for(i=0;i<uchars;i++)
	pBuf[i] = SPI_RW(0);    // 
	CSN = 1;                           
}
/*********************************************************************************************************
/*����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
/*********************************************************************************************************/
void SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
{
	uchar status,i;
	CSN = 0;            //SPIʹ��       
	status = SPI_RW(reg);   
	for(i=0; i<uchars;i++) //
		SPI_RW(*pBuf++);
	CSN = 1;           //�ر�SPI
}
/***********************************************************************************************************
/*���ܣ����� TX_buf������
/**********************************************************************************************************/
void nRF24L01_TX_Mode(unsigned char * TX_buf)
{
	CE=0;			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, TX_buf, PLOAD_WIDTH);// װ������	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);// IRQ�շ�����ж���Ӧ��16λCRC������
	CE=1;		 //�ø�CE���������ݷ���
	delays(250);//ģ�鷢����ʱ��Ҫ120-250֮��
//	Delay(250);//ģ�鷢����ʱ��Ҫ120-250֮��
}
/****************************************************************************************************/
/*���ܣ����ݽ���ģʽ 
/****************************************************************************************************/
void nRF24L01_RX_Mode(unsigned char * RX_buf)
{
	CE=0;
    SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);    // д���ص�ַ	
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);// IRQ�շ�����ж���Ӧ��16λCRC������
	sta=SPI_Read(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	if(RX_DR)				// �ж��Ƿ���յ�����
	SPI_Read_Buf(RD_RX_PLOAD,RX_buf,PLOAD_WIDTH);	//��ȡ������ɱ�־
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	CE = 1;
    delays(200);
	//Delay(200);Delay(200);
}
/****************************************************************************************
  NRF24L01��ʼ��
//***************************************************************************************/
void init_NRF24L01(void)
{

	CE  = 0;        // ����
	CSN = 1;        // SPI��ֹ
	SCK = 0;        // SPIʱ���õ�
	delays(200);delays(200);delays(200);
 	CE=0;    // chip enable
 	CSN=1;   // Spi  disable 
 	SCK=0;   // 
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS,ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS,ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  �������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, PLOAD_WIDTH); //���ý������ݳ��ȣ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
    delays(200);delays(200);
}
