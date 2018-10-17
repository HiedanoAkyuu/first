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

//传感器名字宏定义
#define  OV7725 1

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		160 // <=320
#define  OV7725_WINDOW_HEIGHT		120 // <=240

extern u8 ov_sta;	//在exit.c里 面定义
u8 gm_red, gm_green, gm_blue;

vu16 color_flag[120][160]={0};//


//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u16 line=0, column=0;
	u16 color_finish_flag=0;
 	u16 color;	 
	u16 i;
	
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(L2R_U2D);		//从上到下,从左到右U2D_L2R   从左到右，从上到下L2R_U2D
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
//****
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		//这里的复位是每一帧可以正常读的依据
		//数据都是8位8位的传输
		while (1)
			{
				OV7725_RCK_L;//读数据时钟
				color=GPIOC->IDR&0XFF;	//读数据,,IDR寄存器有地址自增功能
				OV7725_RCK_H; 
				color<<=8; 
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				
				//二值化
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
				color_flag[line][column]=color; //写颜色数据到二维数组 
				column++;
				if (column==160)//160列
				{
					line++;
					column=0;
				}
				if (line==120)//120行
				{
					line=0;
					column=0;
					color_finish_flag++;//至此完成所有图像的采集
					break;
				}
		}
			
		for (i=0;i<19200;i++)
		{
			if (color_finish_flag==1)//写数据
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
					//至此完成所有数据的写入
				}
			}
		}
		memset(color_flag,0,sizeof(color_flag));
 		ov_sta=0;					//清零帧中断标记，开始下一次采集
		
		}
	} 


 int main(void)
 {	     
	u8 effect=0;//这个变量对于显示有影响，，影响暂时未知

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200  这好像没什么用
 	LED_Init();		  			//初始化与LED连接的硬件接口
	LCD_Init();			   		//初始化LCD  

	OV7725_Init();
	OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//QVGA模式输出
	delay_ms(5);		
	OV7725_Special_Effects(effect);//这句话对VGA输出有影响
	OV7725_CS=0;
									  
	EXTI8_Init();						//使能外部中断8,捕获帧中断		
	LCD_Clear(BLACK);						 	 
 	while(1)
	{		 
		OV7725_camera_refresh();		//更新显示
	}	   
}













