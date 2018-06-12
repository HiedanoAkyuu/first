#include <exit.h>
#include <delay.h>
#include <usart.h>
#include <sys.h>
#include <l298n.h>
#include <string.h>


void EXTIINIT()
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);

	//GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

	void EXTI2_IRQ1(void)
	{
		PBout(5)=0;
			u8 len;
			if(USART_RX_STA&0x8000)
			{
				len=USART_RX_STA&0X7FFF;
				USART_RX_BUF[len]=0;
				if(strcmp((char*)USART_RX_BUF,"a")==0)
				{
					IN1=1;
					IN2=0;
				}
			}		
	}
