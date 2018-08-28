#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "exti.h"
#include <stm32f10x.h>

//传感器名字宏定义
#define  OV7725 1

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//在exit.c里 面定义
u8 gm_red, gm_green, gm_blue;
u16 Array[60] ={
	 /*采集像素点矩阵【（46,150），（49,154）】 320列*/
	14870,14871,14872,14873,14874,
	15190,15191,15192,15193,15194,
	15510,15511,15512,15513,15514,
	15830,15831,15832,15833,15834,
	 /*采集像素点矩阵【（100,10），（104,13）】 320列*/
	32010,32011,32012,32013,			 
	32330,32331,32332,32333,
	32650,32651,32652,32653,
	32970,32971,32972,32973,
	33290,33291,33292,33293,
	/*采集像素点矩阵【（165,150），（168,154）】 320列  */
	52950,52951,52952,52953,52954,		 
	53270,53271,53272,53273,53274,
	53590,53591,53592,53593,53594,
	53910,53911,53912,53913,53914,

};

//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u8 whitepoint = 0;
	u8 leftblackpoint  = 0;
	u8 rightblackpoint = 0;
	u8 JudgeFlag = 0;
	u32 i;
	u8 k = 0;
 	u16 color;	 
	
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		for(i=0;i<76800;i++)
		{
				OV7725_RCK_L;//读数据时钟，为什么这里没有设置上拉输入
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				
				//二值化
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
				LCD->LCD_RAM=color; //写颜色数据到LCD 
				
				if(i == Array[k]&&(k<=60))
			{
				if(k < 20)
				{   
					 if(!(color&0x8000))  //检测底片黑，如果bit15为0，则黑点
						leftblackpoint++;	
				}
				else if(k < 40)
				{
					if(color&0x8000)
					whitepoint++;		//测得白点	
				}
				else if(k < 60)
				{
						if(!(color&0x8000))  //检测底片白，如果bit15为0，则黑点
						rightblackpoint++;		
				}														   	 
			k++;
			}		
		}
 		ov_sta=0;					//清零帧中断标记，开始下一次采集
		k=0;
		
		if(whitepoint<=13)	    //中间端未检测到白色则停止
		{
			PEout(5)=0;
		}
		else 
		{
			if((whitepoint>13)&&(leftblackpoint>13)&&(rightblackpoint>13))//如果左端块白色  中端块黑色 右端块白色 则前进
			{
				PBout(5)=0;
			}
		}	
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













