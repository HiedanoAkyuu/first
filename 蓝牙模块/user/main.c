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
		if(key==KEY1_PRES)						//切换模块主从设置
		{
   			key=HC05_Get_Role();
			if(key!=0XFF)
			{
				key=!key;  					//状态取反	   
				if(key==0)HC05_Set_Cmd("AT+ROLE=0");
				else HC05_Set_Cmd("AT+ROLE=1");
				HC05_Set_Cmd("AT+RESET");	//复位ATK-HC05模块
				delay_ms(200);
			}
		}else if(key==KEY0_PRES)
		{
			sendmask=!sendmask;				//发送/停止发送  	 
		}else delay_ms(10);	   
		if(t==50)
		{
			if(sendmask)					//定时发送
			{
				sprintf((char*)sendbuf,"ALIENTEK HC05 %d\r\n",sendcnt);
				u3_printf("ALIENTEK HC05 %d\r\n",sendcnt);		//发送到蓝牙模块
				sendcnt++;
				if(sendcnt>99)sendcnt=0;
			} 	  
			t=0;
			PBout(5)=!PBout(5); 	     
		}	  
		if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  	USART3_RX_BUF[reclen]=0;	 	//加入结束符
			if(reclen==9||reclen==8) 		//控制DS1检测
			{
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 ON")==0)PBout(5)=0;	//打开LED1
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 OFF")==0)PEout(5)=1;//关闭LED1
			}
 			USART3_RX_STA=0;	 
		}	 															     				   
		t++;	
	}
	
	
	
}
