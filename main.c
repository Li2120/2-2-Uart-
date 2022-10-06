#include <stdio.h>
#include "uart.h"

void	GPIO_config(void)
{
	P5M0 = 0;
	P5M1 = 0;
}
/*----------------------------
接线：RX》TX    TX》RX
Uart1  P36 RX1 P37 TX1  
Uart2  P10 RX2 P11 TX2
Uart3  P00 RX3 P01 TX3
Uart4  P02 RX4 P03 TX4
----------------------------*/
void main(void)
{
	GPIO_config();
	UartInit();
	Uart4_SendString("Please enter your command:");
	delayms(200);

	while (1) {
		if(Uart4_Hand("ledon")){
			LED = LED_ON;
			Uart4_CLR_Buf();
			Uart4_SendString("LED ON!\r\t");
		}else if(Uart4_Hand("ledoff")) {
			LED = LED_OFF;
			Uart4_CLR_Buf();
			Uart4_SendString("LED OFF!\r\t");
		}
	}
}
