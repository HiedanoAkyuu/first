#include <stm32f10x.h>
#include <led.h>
#include <key.h>
#include <sys.h>
#include <usart.h>
#include <delay.h>
#include <stdio.h>

//int fputc(int ch,FILE*f)
//{
//	USART_SendData(USART1,(unsigned char)ch);
//	while(!(USART1->SR&USART_FLAG_TXE));
//	return(ch);
//}

//int getkey(void)
//{
//	while(!(USART1->SR&USART_FLAG_RXNE));
//	return ((int)(USART1->DR&0X1FF));
//}

 int main(void)
 {	
	 u16 t,len;//定义t,len两个变量
	 u16 times=0;//定义times并赋值0
	 delay_init(72);//初始化外部时钟为72MHZ
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2：2位抢占优先级，2位响应优先级
	 uart_init(72,115200);//串口初始化位115200,PCLK用72Mhz
	 LED_Init();//LED端口初始化
	 KEY_Init();//KEY端口初始化
	 while(1)
	 {
		 if(USART_RX_STA&0x8000)//判断是否接收数据，如果有则进入次循环
		 {
			 len=USART_RX_STA&0x3fff;//赋予len数据的长度
			 {
				 printf("\r\n你发送的是:\r\n\r\n");
         for(t=0;t<len;t++)
         { USART_SendData(USART1, USART_RX_BUF[t]);
           while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
         }
       printf("\r\n\r\n");
       USART_RX_STA=0;
			 }
		 }
		 else//否则进入此循环，边亮灯边用串口输出
		 {
			 times++;
			 if(times%5000==0)
			 {
				 printf("\r\n精英STM32开发版 串口实验\r\n");
				 printf("正点原子@ALIENTEK\r\n\r\n");
			 }
			 if(times%200==0)printf("请输入数据，回车键结束\n");
			 if(times%30==0) PBout(5)=!PBout(5);//闪灯
			 delay_ms(10);
		 }
	   
	 }
		 
	 }
	 
