#include<led.h>
#include<stm32f10x.h>

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initaaa;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��GPIOB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//GPIOE
	
	GPIO_Initaaa.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Initaaa.GPIO_Pin=GPIO_Pin_5;//GPIOB5��
	GPIO_Initaaa.GPIO_Speed=GPIO_Speed_50MHz;//����ٶ�50M
	GPIO_Init(GPIOB,&GPIO_Initaaa);
  GPIO_SetBits(GPIOB,GPIO_Pin_5);

	
	GPIO_Initaaa.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Initaaa.GPIO_Pin=GPIO_Pin_5;//GPIOB5��
	GPIO_Initaaa.GPIO_Speed=GPIO_Speed_50MHz;//����ٶ�50M
	GPIO_Init(GPIOE,&GPIO_Initaaa);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	//���io������
}

