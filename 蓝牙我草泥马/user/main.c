#include <sys.h>
#include <usart3.h>
#include <key.h>
#include <timer.h>
#include <hc05.h>
#include <stdio.h>

//��ʵ��Ŀ�ģ���������LED1+ON���źź�PBout(5)����LED+OFF�󣬰���LED2+ON��PEout(5)��



int main()
{
	u8 reclen=0;
	
	delay_init();//��ʼ����ʱ����������Ҫ�򿪵Ŀ�
	LED_Init();//��ʼ��LED
	usart3_init(115200);//��ʼ��usart3
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�
	
	delay_ms(1000);//�ȴ����������ȶ�
	
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
