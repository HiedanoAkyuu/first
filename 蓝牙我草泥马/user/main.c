#include <sys.h>
#include <usart3.h>
#include <key.h>
#include <timer.h>
#include <hc05.h>
#include <stdio.h>

//本实验目的：蓝牙接收LED1+ON的信号后，PBout(5)亮，LED+OFF后，暗。LED2+ON后，PEout(5)亮



int main()
{
	u8 reclen=0;
	
	delay_init();//初始化延时函数，打开需要打开的口
	LED_Init();//初始化LED
	usart3_init(115200);//初始化usart3
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级
	
	delay_ms(1000);//等待蓝牙电流稳定
	
	while(1)
	{
	if (USART3_RX_STA&0X8000)
	{
//		reclen=USART3_RX_STA&0X7FFF;
//		USART3_RX_BUF[reclen]=0;
//		if(reclen==9||reclen==8)
//			{
//				if(strcmp((const char*)USART3_RX_BUF,"O")==0)PBout(5)=0;
//				if(strcmp((const char*)USART3_RX_BUF,"N")==1)PBout(5)=1;
//			}
//		USART3_RX_STA=0;
		
		PBout(5)=0;
		delay_ms(100);
		PBout(5)=1;
	}
  }

}
