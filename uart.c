#include "uart.h"
#include <string.h>

#define Uart1_Buf_Max 20               //串口数据缓存长度
u8 idata  Uart1_Rec_Buf[Uart1_Buf_Max];  //串口数据缓存
u8 point1 = 0;             //绶存指针

#define Uart2_Buf_Max 20               //串口数据缓存长度
u8 xdata  Uart2_Rec_Buf[Uart2_Buf_Max];  //串口数据缓存
u8 point2 = 0;             //绶存指针
bit	B_TX2_Busy = 0;	//发送忙标志

#define Uart3_Buf_Max 20               //串口数据缓存长度
u8 xdata  Uart3_Rec_Buf[Uart3_Buf_Max];  //串口数据缓存
u8 point3 = 0;             //绶存指针
bit	B_TX3_Busy = 0;	//发送忙标志

#define Uart4_Buf_Max 20               //串口数据缓存长度
u8 xdata  Uart4_Rec_Buf[Uart4_Buf_Max];  //串口数据缓存
u8 point4 = 0;             //绶存指针
bit	B_TX4_Busy = 0;	//发送忙标志

/*----------------------------
延时函数
----------------------------*/
void delayms(u16 ms){
	unsigned int i;
	 do{
	    i = MAIN_Fosc / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

/*----------------------------
定时器函数 STC_ISP生成
----------------------------*/
void UartInit(void)		//9600bps@11.0592MHz
{

	P3M0 &= ~(1<<7);		//设置P3.7为准双向IO
	P3M1 &= ~(1<<7);
	
	S1_USE_P36P37();

	PCON &= 0x7F;			//波特率不倍速
	SCON = 0x50;			//8位数据,可变波特率
	AUXR |= 0x40;			//定时器时钟1T模式
	AUXR &= 0xFE;			//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;			//设置定时器模式
	TMOD |= 0x20;			//设置定时器模式
	TL1 = 0xDC;				//设置定时初始值
	TH1 = 0xDC;				//设置定时重载值
	ET1 = 0;					//禁止定时器%d中断
	TR1 = 1;					//定时器1开始计时

	S2CON = 0x50;			//8位数据,可变波特率
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0xE0;				//设置定时初始值
	T2H = 0xFE;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时

	S3CON = 0x10;			//8位数据,可变波特率
	S3CON |= 0x40;		//串口3选择定时器3为波特率发生器
	T4T3M |= 0x02;		//定时器时钟1T模式
	T3L = 0xE0;				//设置定时初始值
	T3H = 0xFE;				//设置定时初始值
	T4T3M |= 0x08;		//定时器3开始计时

	S4CON = 0x10;			//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器时钟1T模式
	T4L = 0xE0;				//设置定时初始值
	T4H = 0xFE;				//设置定时初始值
	T4T3M |= 0x80;		//定时器4开始计时

	UART2_INT_ENABLE();			//允许串口2中断
	UART3_INT_ENABLE();			//允许串口3中断
	UART4_INT_ENABLE();			//允许串口4中断

	ES = 1;					//开启中断
	EA = 1;					//允许中断
}

/*----------------------------
通过串口1发送串口数据
----------------------------*/
void Uart1_SendData(unsigned char ch){
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
通过串口1发送串口字符串
----------------------------*/
void Uart1_SendString(char *s){
    while (*s){                  //检测字符串结束标志
			Uart1_SendData(*s++);         //发送当前字符
    }
}

/*----------------------------
串口命令识别函数
----------------------------*/
bit  Uart1_Hand(unsigned char *a){
	if(strstr(Uart1_Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}

/*----------------------------
串口缓存清理
----------------------------*/
void Uart1_CLR_Buf(void){
	memset(Uart1_Rec_Buf,0,Uart1_Buf_Max);
	point1 = 0;
}

/*----------------------------
串口中断函数
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
通过串口2发送串口数据
----------------------------*/
void Uart2_SendData(unsigned char ch){
	while(B_TX2_Busy); //不忙时候发送数据
	S2BUF = ch;
	B_TX2_Busy = 1;
}

/*----------------------------
通过串口2发送字符串
----------------------------*/
void Uart2_SendString(char *s){
	while(*s){
		Uart2_SendData(*s++); //发送当前字符
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
通过串口3发送串口数据
----------------------------*/
void Uart3_SendData(unsigned char ch){
	while(B_TX3_Busy); //不忙时候发送数据
	S3BUF = ch;
	B_TX3_Busy = 1;
}

/*----------------------------
通过串口3发送字符串
----------------------------*/
void Uart3_SendString(char *s){
	while(*s){
		Uart3_SendData(*s++); //发送当前字符
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
通过串口4发送串口数据
----------------------------*/
void Uart4_SendData(unsigned char ch){
	while(B_TX4_Busy); //不忙时候发送数据
	S4BUF = ch;
	B_TX4_Busy = 1;
}

/*----------------------------
通过串口4发送字符串
----------------------------*/
void Uart4_SendString(char *s){
	while(*s){
		Uart4_SendData(*s++); //发送当前字符
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
