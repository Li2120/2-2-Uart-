#ifndef __UART_H
#define __UART_H

#include	"STC15.H"

#define 	MAIN_Fosc		11059200L									//

#define		S1_USE_P30P31()		P_SW1 &= ~0xc0					//UART1 使用P30 P31口	默认
#define		S1_USE_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 使用P36 P37口
#define		S1_USE_P16P17()		P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 使用P16 P17口

#define		TI2					(S2CON & 2) != 0
#define		RI2					(S2CON & 1) != 0
#define		SET_TI2()			S2CON |=  2
#define		CLR_TI2()			S2CON &= ~2
#define		CLR_RI2()			S2CON &= ~1

#define		UART2_INT_ENABLE()		IE2 |=  1	//允许串口2中断
#define		UART2_INT_DISABLE()		IE2 &= ~1	//不允许串口2中断

#define		TI3					(S3CON & 2) != 0
#define		RI3					(S3CON & 1) != 0
#define		SET_TI3()			S3CON |=  2
#define		CLR_TI3()			S3CON &= ~2
#define		CLR_RI3()			S3CON &= ~1

#define		UART3_INT_ENABLE()		IE2 |=  (1<<3)	//允许串口3中断
#define		UART3_INT_DISABLE()		IE2 &= ~(1<<3)	//不允许串口3中断

#define		TI4					(S4CON & 2) != 0
#define		RI4					(S4CON & 1) != 0
#define		SET_TI4()			S4CON |=  2
#define		CLR_TI4()			S4CON &= ~2
#define		CLR_RI4()			S4CON &= ~1

#define		UART4_INT_ENABLE()		IE2 |=  (1<<4)	//允许串口3中断
#define		UART4_INT_DISABLE()		IE2 &= ~(1<<4)	//不允许串口3中断

sbit LED = P5^5;

#define LED_ON 0
#define LED_OFF 1


typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

void delayms(u16 ms);														//延时
void UartInit(void);														//初始化
void GPIO_config(void);
/*----------------------------
Uart1
----------------------------*/
void Uart1_SendData(unsigned char ch);											//发送串口
void Uart1_SendString(char *s);									//发送字符串
bit  Uart1_Hand(unsigned char *a) ;
void Uart1_CLR_Buf(void) ;

/*----------------------------
Uart2
----------------------------*/
void Uart2_SendData(unsigned char ch);
void Uart2_SendString(char *s);
bit  Uart2_Hand(unsigned char *a);
void Uart2_CLR_Buf(void);

/*----------------------------
Uart3
----------------------------*/
void Uart3_SendData(unsigned char ch);
void Uart3_SendString(char *s);
bit  Uart3_Hand(unsigned char *a);
void Uart3_CLR_Buf(void);

/*----------------------------
Uart4
----------------------------*/
void Uart4_SendData(unsigned char ch);
void Uart4_SendString(char *s);
bit  Uart4_Hand(unsigned char *a);
void Uart4_CLR_Buf(void);

#endif
