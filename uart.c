#include "uart.h"
#include <string.h>

#define Uart1_Buf_Max 20               //�������ݻ��泤��
u8 idata  Uart1_Rec_Buf[Uart1_Buf_Max];  //�������ݻ���
u8 point1 = 0;             //練�ָ��

#define Uart2_Buf_Max 20               //�������ݻ��泤��
u8 xdata  Uart2_Rec_Buf[Uart2_Buf_Max];  //�������ݻ���
u8 point2 = 0;             //練�ָ��
bit	B_TX2_Busy = 0;	//����æ��־

#define Uart3_Buf_Max 20               //�������ݻ��泤��
u8 xdata  Uart3_Rec_Buf[Uart3_Buf_Max];  //�������ݻ���
u8 point3 = 0;             //練�ָ��
bit	B_TX3_Busy = 0;	//����æ��־

#define Uart4_Buf_Max 20               //�������ݻ��泤��
u8 xdata  Uart4_Rec_Buf[Uart4_Buf_Max];  //�������ݻ���
u8 point4 = 0;             //練�ָ��
bit	B_TX4_Busy = 0;	//����æ��־

/*----------------------------
��ʱ����
----------------------------*/
void delayms(u16 ms){
	unsigned int i;
	 do{
	    i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

/*----------------------------
��ʱ������ STC_ISP����
----------------------------*/
void UartInit(void)		//9600bps@11.0592MHz
{

	P3M0 &= ~(1<<7);		//����P3.7Ϊ׼˫��IO
	P3M1 &= ~(1<<7);
	
	S1_USE_P36P37();

	PCON &= 0x7F;			//�����ʲ�����
	SCON = 0x50;			//8λ����,�ɱ䲨����
	AUXR |= 0x40;			//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;			//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;			//���ö�ʱ��ģʽ
	TMOD |= 0x20;			//���ö�ʱ��ģʽ
	TL1 = 0xDC;				//���ö�ʱ��ʼֵ
	TH1 = 0xDC;				//���ö�ʱ����ֵ
	ET1 = 0;					//��ֹ��ʱ��%d�ж�
	TR1 = 1;					//��ʱ��1��ʼ��ʱ

	S2CON = 0x50;			//8λ����,�ɱ䲨����
	AUXR |= 0x04;			//��ʱ��ʱ��1Tģʽ
	T2L = 0xE0;				//���ö�ʱ��ʼֵ
	T2H = 0xFE;				//���ö�ʱ��ʼֵ
	AUXR |= 0x10;			//��ʱ��2��ʼ��ʱ

	S3CON = 0x10;			//8λ����,�ɱ䲨����
	S3CON |= 0x40;		//����3ѡ��ʱ��3Ϊ�����ʷ�����
	T4T3M |= 0x02;		//��ʱ��ʱ��1Tģʽ
	T3L = 0xE0;				//���ö�ʱ��ʼֵ
	T3H = 0xFE;				//���ö�ʱ��ʼֵ
	T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ

	S4CON = 0x10;			//8λ����,�ɱ䲨����
	S4CON |= 0x40;		//����4ѡ��ʱ��4Ϊ�����ʷ�����
	T4T3M |= 0x20;		//��ʱ��ʱ��1Tģʽ
	T4L = 0xE0;				//���ö�ʱ��ʼֵ
	T4H = 0xFE;				//���ö�ʱ��ʼֵ
	T4T3M |= 0x80;		//��ʱ��4��ʼ��ʱ

	UART2_INT_ENABLE();			//������2�ж�
	UART3_INT_ENABLE();			//������3�ж�
	UART4_INT_ENABLE();			//������4�ж�

	ES = 1;					//�����ж�
	EA = 1;					//�����ж�
}

/*----------------------------
ͨ������1���ʹ�������
----------------------------*/
void Uart1_SendData(unsigned char ch){
    SBUF = ch;                 //д���ݵ�UART���ݼĴ���
		while(TI == 0);
		TI = 0;
}

/*----------------------------
ͨ������1���ʹ����ַ���
----------------------------*/
void Uart1_SendString(char *s){
    while (*s){                  //����ַ���������־
			Uart1_SendData(*s++);         //���͵�ǰ�ַ�
    }
}

/*----------------------------
��������ʶ����
----------------------------*/
bit  Uart1_Hand(unsigned char *a){
	if(strstr(Uart1_Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}

/*----------------------------
���ڻ�������
----------------------------*/
void Uart1_CLR_Buf(void){
	memset(Uart1_Rec_Buf,0,Uart1_Buf_Max);
	point1 = 0;
}

/*----------------------------
�����жϺ���
----------------------------*/
void Uart1() interrupt 4{
	ES = 0;
	if(RI){
		RI = 0;
		Uart1_Rec_Buf[point1] = SBUF;
		point1++;
		if(point1 >= Uart1_Buf_Max){
			point1 = 0;
		}
	}
	if(TI){
			TI = 0;
		}
		ES = 1;
}

/*----------------------------
ͨ������2���ʹ�������
----------------------------*/
void Uart2_SendData(unsigned char ch){
	while(B_TX2_Busy); //��æʱ��������
	S2BUF = ch;
	B_TX2_Busy = 1;
}

/*----------------------------
ͨ������2�����ַ���
----------------------------*/
void Uart2_SendString(char *s){
	while(*s){
		Uart2_SendData(*s++); //���͵�ǰ�ַ�
	}
}

bit  Uart2_Hand(unsigned char *a){
	if(strstr(Uart2_Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}

void Uart2_CLR_Buf(void){
	memset(Uart2_Rec_Buf,0,Uart2_Buf_Max);
	point2 = 0;
}

void Uart2() interrupt 8{
	UART2_INT_DISABLE();
	if(RI2){
		CLR_RI2();
		Uart2_Rec_Buf[point2] = S2BUF;
		point2++;
		if(point2 >= Uart2_Buf_Max){
			point2 = 0;
		}
	}
	if(TI2){
		CLR_TI2();
		B_TX2_Busy = 0;
	}
	UART2_INT_ENABLE();
}

/*----------------------------
ͨ������3���ʹ�������
----------------------------*/
void Uart3_SendData(unsigned char ch){
	while(B_TX3_Busy); //��æʱ��������
	S3BUF = ch;
	B_TX3_Busy = 1;
}

/*----------------------------
ͨ������3�����ַ���
----------------------------*/
void Uart3_SendString(char *s){
	while(*s){
		Uart3_SendData(*s++); //���͵�ǰ�ַ�
	}
}

bit  Uart3_Hand(unsigned char *a){
	if(strstr(Uart3_Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}

void Uart3_CLR_Buf(void){
	memset(Uart3_Rec_Buf,0,Uart3_Buf_Max);
	point3 = 0;
}

void Uart3() interrupt 17 {
	UART3_INT_DISABLE();
	if(RI3){
		CLR_RI3();
		Uart3_Rec_Buf[point3] = S3BUF;
		point3++;
		if(point3>=Uart3_Buf_Max){
			point3 = 0;
		}
	}
	if(TI3){
		CLR_TI3();
		B_TX3_Busy = 0;
	}
	UART3_INT_ENABLE();
}
/*----------------------------
ͨ������4���ʹ�������
----------------------------*/
void Uart4_SendData(unsigned char ch){
	while(B_TX4_Busy); //��æʱ��������
	S4BUF = ch;
	B_TX4_Busy = 1;
}

/*----------------------------
ͨ������4�����ַ���
----------------------------*/
void Uart4_SendString(char *s){
	while(*s){
		Uart4_SendData(*s++); //���͵�ǰ�ַ�
	}
}

bit  Uart4_Hand(unsigned char *a){
	if(strstr(Uart4_Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}

void Uart4_CLR_Buf(void){
	memset(Uart4_Rec_Buf,0,Uart4_Buf_Max);
	point4 = 0;
}

void Uart4() interrupt 18 {
	UART4_INT_DISABLE();
	if(RI4){
		CLR_RI4();
		Uart4_Rec_Buf[point4] = S4BUF;
		point4++;
		if(point4>=Uart4_Buf_Max){
			point4 = 0;
		}
	}
	if(TI4){
		CLR_TI4();
		B_TX4_Busy = 0;
	}
	UART4_INT_ENABLE();
}
