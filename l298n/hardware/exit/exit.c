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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);

	//GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

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
