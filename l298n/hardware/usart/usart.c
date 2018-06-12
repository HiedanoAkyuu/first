#include <delay.h>
#include <usart.h>
#include <stdio.h>
#include <l298n.h>
#include <led.h>

void USART_INIT(void)
{
  GPIO_InitTypeDef GPIOINIT;
	USART_InitTypeDef USARTINIT;
	NVIC_InitTypeDef NVICINIT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOBʱ��
	
	
	USART_DeInit(USART1);//��λ����1

	//PA9 RX
	GPIOINIT.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOINIT.GPIO_Pin=GPIO_Pin_9;
	GPIOINIT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOINIT);
	
	//PA10 TX
	GPIOINIT.GPIO_Pin = GPIO_Pin_10;
  GPIOINIT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIOINIT);
	
	USART_Init(USART1, &USARTINIT);
	USARTINIT.USART_BaudRate = 115200;
	USARTINIT.USART_WordLength=USART_WordLength_8b;
	USARTINIT.USART_StopBits=USART_StopBits_1;
	USARTINIT.USART_Parity=USART_Parity_No;
	USARTINIT.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USARTINIT.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	
	NVICINIT.NVIC_IRQChannel = USART1_IRQn;
  NVICINIT.NVIC_IRQChannelPreemptionPriority=3 ;
  NVICINIT.NVIC_IRQChannelSubPriority = 3;
  NVICINIT.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVICINIT);//�����жϣ������жϷ��� 
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ����ж�
	
	USART_Cmd(USART1, ENABLE);//����ʹ��
}

//����һ���ַ�
void USARTSENDBYTE(USART_TypeDef *USARTx, uint8_t ch)
{
	USART_SendData(USART1,ch);

	while((USART_GetFlagStatus(USART1,USART_FLAG_TXE))==RESET);

}

//�����ַ���
void USARTSENDSTRING(USART_TypeDef *USARTx, char *str)
{
	u8 k=0;
	do
	{
		USARTSENDBYTE(USART1,*(str+k));
		k++;
	}
	while(*(str+k)!='\0');
	
	while((USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET));
}


void USART_IRQHandler(void)
{
	u8 aa;
	if((USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		
	}



}




