#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "exti.h"
#include <stm32f10x.h>

//���������ֺ궨��
#define  OV7725 1

//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//��exit.c�� �涨��
u8 gm_red, gm_green, gm_blue;
u16 Array[60] ={
	 /*�ɼ����ص���󡾣�46,150������49,154���� 320��*/
	14870,14871,14872,14873,14874,
	15190,15191,15192,15193,15194,
	15510,15511,15512,15513,15514,
	15830,15831,15832,15833,15834,
	 /*�ɼ����ص���󡾣�100,10������104,13���� 320��*/
	32010,32011,32012,32013,			 
	32330,32331,32332,32333,
	32650,32651,32652,32653,
	32970,32971,32972,32973,
	33290,33291,33292,33293,
	/*�ɼ����ص���󡾣�165,150������168,154���� 320��  */
	52950,52951,52952,52953,52954,		 
	53270,53271,53272,53273,53274,
	53590,53591,53592,53593,53594,
	53910,53911,53912,53913,53914,

};

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
	u8 whitepoint = 0;
	u8 leftblackpoint  = 0;
	u8 rightblackpoint = 0;
	u8 JudgeFlag = 0;
	u32 i;
	u8 k = 0;
 	u16 color;	 
	
	if(ov_sta)//��֡�жϸ���
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H; 
		for(i=0;i<76800;i++)
		{
				OV7725_RCK_L;//������ʱ�ӣ�Ϊʲô����û��������������
				color=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				
				//��ֵ��
				gm_red = color>>11;
				gm_green = (color&0x07E0)>>5;
				gm_blue = (color&0x001F);
				if((gm_red<0X0008)&&(gm_blue<0X0020)&&(gm_green<0X0020))
				{    
					color =0x0000;    
				}
				else
				{   
					color=0xffff;
				} 
				LCD->LCD_RAM=color; //д��ɫ���ݵ�LCD 
				
				if(i == Array[k]&&(k<=60))
			{
				if(k < 20)
				{   
					 if(!(color&0x8000))  //����Ƭ�ڣ����bit15Ϊ0����ڵ�
						leftblackpoint++;	
				}
				else if(k < 40)
				{
					if(color&0x8000)
					whitepoint++;		//��ð׵�	
				}
				else if(k < 60)
				{
						if(!(color&0x8000))  //����Ƭ�ף����bit15Ϊ0����ڵ�
						rightblackpoint++;		
				}														   	 
			k++;
			}		
		}
 		ov_sta=0;					//����֡�жϱ�ǣ���ʼ��һ�βɼ�
		k=0;
		
		if(whitepoint<=13)	    //�м��δ��⵽��ɫ��ֹͣ
		{
			PEout(5)=0;
		}
		else 
		{
			if((whitepoint>13)&&(leftblackpoint>13)&&(rightblackpoint>13))//�����˿��ɫ  �ж˿��ɫ �Ҷ˿��ɫ ��ǰ��
			{
				PBout(5)=0;
			}
		}	
	} 
}


 int main(void)
 {	     
	u8 effect=0;//�������������ʾ��Ӱ�죬��Ӱ����ʱδ֪

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200  �����ûʲô��
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   		//��ʼ��LCD  

	OV7725_Init();
	OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//QVGAģʽ���
	delay_ms(5);		
	OV7725_Special_Effects(effect);//��仰��VGA�����Ӱ��
	OV7725_CS=0;
									  
	EXTI8_Init();						//ʹ���ⲿ�ж�8,����֡�ж�		
	LCD_Clear(BLACK);						 	 
 	while(1)
	{		 
		OV7725_camera_refresh();		//������ʾ
	}	   
}













