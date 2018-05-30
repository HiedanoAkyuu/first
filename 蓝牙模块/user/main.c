#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "hc05.h"
#include "usart3.h"			 	 
#include "string.h"	


int main()
{
	u8 t;
	u8 key;
	u8 sendmask=0;
	u8 sendcnt=0;
	u8 sendbuf[20];	  
	u8 reclen=0;  	
	delay_init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(72,115200);
	LED_Init();
	KEY_Init();
	
	
	delay_ms(1000);	
	HC05_Init();
	USART3_RX_STA=0;
	while(1)
	{
	key=KEY_Scan(0);
		if(key==KEY1_PRES)						//�л�ģ����������
		{
   			key=HC05_Get_Role();
			if(key!=0XFF)
			{
				key=!key;  					//״̬ȡ��	   
				if(key==0)HC05_Set_Cmd("AT+ROLE=0");
				else HC05_Set_Cmd("AT+ROLE=1");
				HC05_Set_Cmd("AT+RESET");	//��λATK-HC05ģ��
				delay_ms(200);
			}
		}else if(key==KEY0_PRES)
		{
			sendmask=!sendmask;				//����/ֹͣ����  	 
		}else delay_ms(10);	   
		if(t==50)
		{
			if(sendmask)					//��ʱ����
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
				u3_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//���͵�����ģ��
				sendcnt++;
				if(sendcnt>99)sendcnt=0;
			} 	  
			t=0;
			PBout(5)=!PBout(5); 	     
		}	  
		if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  	USART3_RX_BUF[reclen]=0;	 	//���������
			if(reclen==9||reclen==8) 		//����DS1���
			{
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 ON")==0)PBout(5)=0;	//��LED1
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 OFF")==0)PEout(5)=1;//�ر�LED1
			}
 			USART3_RX_STA=0;	 
		}	 															     				   
		t++;	
	}
	
	
	
}
