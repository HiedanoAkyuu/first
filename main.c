#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "ov7725.h"
#include "exti.h"
#include "stm32f10x.h"
#include "l298n.h"
#include "string.h"

//���������ֺ궨��
#define  OV7725 1

//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		160 // <=320
#define  OV7725_WINDOW_HEIGHT		120 // <=240

extern u8 ov_sta;	//��exit.c�� �涨��
u8 gm_red, gm_green, gm_blue;

vu16 color_flag[120][160]={0};//


//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
	u16 line=0, column=0;
	u16 color_finish_flag=0;
 	u16 color;	 
	u16 i;
	
	if(ov_sta)//��֡�жϸ���
	{
		LCD_Scan_Dir(L2R_U2D);		//���ϵ���,������U2D_L2R   �����ң����ϵ���L2R_U2D
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
//****
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H; 
		//����ĸ�λ��ÿһ֡����������������
		//���ݶ���8λ8λ�Ĵ���
		while (1)
			{
				OV7725_RCK_L;//������ʱ��
				color=GPIOC->IDR&0XFF;	//������,,IDR�Ĵ����е�ַ��������
				OV7725_RCK_H; 
				color<<=8; 
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				
				//��ֵ��
				gm_red = color>>11;
				gm_green = (color&0x07E0)>>5;
				gm_blue = (color&0x001F);
				if((gm_red<0X0020)&&(gm_blue<0X0025)&&(gm_green<0X0025))
				{    
					color =0x0000;    
				}
				else
				{   
					color=0xffff;
				} 
				color_flag[line][column]=color; //д��ɫ���ݵ���ά���� 
				column++;
				if (column==160)//160��
				{
					line++;
					column=0;
				}
				if (line==120)//120��
				{
					line=0;
					column=0;
					color_finish_flag++;//�����������ͼ��Ĳɼ�
					break;
				}
		}
			
		for (i=0;i<19200;i++)
		{
			if (color_finish_flag==1)//д����
			{
				LCD->LCD_RAM=color_flag[line][column];
				
				line++;
				if (line==120)
				{
					column++;
					line=0;
				}
				if (column==160)
				{
					line=0;
					column=0;
					color_finish_flag=0;
					//��������������ݵ�д��
				}
			}
		}
		memset(color_flag,0,sizeof(color_flag));
 		ov_sta=0;					//����֡�жϱ�ǣ���ʼ��һ�βɼ�
		
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













