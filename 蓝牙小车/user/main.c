#include <stm32f10x.h>
#include <l298n.h>
#include <usart3.h>
#include <led.h>
#include <delay.h>
#include <sys.h>
#include <string.h>
#include <stdio.h>

void right(void)
{
			IN1=1;//×óÂÖ
			IN2=0;
			IN3=0;
			IN4=1;		
}


void left(void)
{
			IN1=0;
			IN2=1;
			IN3=1;
			IN4=0;		
}

void straight(void)
{
			IN1=1;
			IN2=0;
			IN3=1;
			IN4=0;	
}

void back(void)
{
			IN1=0;
			IN2=1;
			IN3=0;
			IN4=1;	
}


int main()
{
	u8 reclen;
	
	LED_Init();
	delay_init(72);
	usart3_init(115200);
	l298n_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_ms(1000);

	while(1)
	{
		if (USART3_RX_STA&0X8000)
		{
			reclen=USART3_RX_STA&0X7FFF;
			USART3_RX_BUF[reclen]=0;
			if(strcmp((const char*)USART3_RX_BUF,"A")==0)
			{
				PBout(5)=0;
				delay_ms(100);
				PBout(5)=1;
				straight();
			}
			if(strcmp((const char*)USART3_RX_BUF,"C")==0)
			{	
				PBout(5)=0;
				delay_ms(100);
				PBout(5)=1;
				right();
			}
			if(strcmp((const char*)USART3_RX_BUF,"E")==0)
			{	
				PBout(5)=0;
				delay_ms(100);
				PBout(5)=1;
				back();
			}
			if(strcmp((const char*)USART3_RX_BUF,"G")==0)
			{
				PBout(5)=0;
				delay_ms(100);
				PBout(5)=1;
				left();
			}
			USART3_RX_STA=0;
		}
	}
}
