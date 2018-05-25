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
	 u16 t,len;//����t,len��������
	 u16 times=0;//����times����ֵ0
	 delay_init(72);//��ʼ���ⲿʱ��Ϊ72MHZ
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart_init(72,115200);//���ڳ�ʼ��λ115200,PCLK��72Mhz
	 LED_Init();//LED�˿ڳ�ʼ��
	 KEY_Init();//KEY�˿ڳ�ʼ��
	 while(1)
	 {
		 if(USART_RX_STA&0x8000)//�ж��Ƿ�������ݣ������������ѭ��
		 {
			 len=USART_RX_STA&0x3fff;//����len���ݵĳ���
			 {
				 printf("\r\n�㷢�͵���:\r\n\r\n");
         for(t=0;t<len;t++)
         { USART_SendData(USART1, USART_RX_BUF[t]);
           while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
         }
       printf("\r\n\r\n");
       USART_RX_STA=0;
			 }
		 }
		 else//��������ѭ���������Ʊ��ô������
		 {
			 times++;
			 if(times%5000==0)
			 {
				 printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
				 printf("����ԭ��@ALIENTEK\r\n\r\n");
			 }
			 if(times%200==0)printf("���������ݣ��س�������\n");
			 if(times%30==0) PBout(5)=!PBout(5);//����
			 delay_ms(10);
		 }
	   
	 }
		 
	 }
	 
