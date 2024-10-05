#include <Uart.h>
//���޲����ʿɴ�230400bps@33.1776MHz�����ǻ���ֺ���
void Uart1_Init(void)	//115200bps@33.1776MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xB8;			//���ö�ʱ��ʼֵ
	TH1 = 0xFF;			//���ö�ʱ��ʼֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	ES = 1;				//ʹ�ܴ���1�ж�
	EA = 1;
}

/* �ֽڷ��ͺ��� */
void SendByte(unsigned char dat)
{
	SBUF=dat;//��dat���ݸ���SBUF�������ݷ��ͳ�ȥ
	while(TI == 0);//�ȴ����ݷ���
	TI = 0;//�����ͱ�־λ����
}

/* �ַ������ͺ��� */
void Uart_Send_String(unsigned char *dat)
{
	while(*dat != '\0')//���ַ���Ϊ��ʱ����������
		SendByte(*dat++);//���ͺ�ָ��dat��1��ָ����һ���ֽ�
}