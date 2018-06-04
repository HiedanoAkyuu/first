#include <sys.h>
#include <usart3.h>
#include <key.h>
#include <timer.h>
#include <hc05.h>
#include <stdio.h>



int main()
{
	u8 reclen=0;
	
	delay_init();
	LED_Init();
	usart3_init(115200);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	delay_ms(1000);
	

	while(1)
	{
	if (USART3_RX_STA&0X8000)
	{
		reclen=USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[reclen]=0;
		if(reclen==9||reclen==8)
			{
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 ON")==0)PBout(5)=0;
				delay_ms(500);
				if(strcmp((const char*)USART3_RX_BUF,"+LED1 OFF")==1)PBout(5)=1;
			}
		USART3_RX_STA=0;
		

	}
  }

}
