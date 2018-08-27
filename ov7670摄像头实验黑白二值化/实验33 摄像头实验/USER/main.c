#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "ov7725.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"

 
/************************************************
 ALIENTEK精英STM32开发板实验33
 摄像头OV7670 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//传感器名字宏定义
#define  OV7725 1
#define  OV7670 2

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义 

//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;
	u8 color_r,color_g,color_b;
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)//这一部分是开窗
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				
				
				//接下来是二值化重点
				color_r=color>>11;
				color_g=(color&0x07ff)>>5;
				color_b=color&0x001f;
				
				color_g=(color_g<<2)*58;
				color_b=(color_b<<3)*11;
				color=(color_r+color_g+color_b+50)/100;
				
				if(color>100){
					LCD->LCD_RAM=0xffff;
				}
				else{
					LCD->LCD_RAM=0;
			}
		}
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}

 int main(void)
 {	 
	u8 sensor=0;
	u8 key;
 	u8 i=0;	    
	u8 msgbuf[15];//消息缓存区
	u8 tm=0;
	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;

	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	TPAD_Init(6);				//触摸按键初始化 
 	POINT_COLOR=RED;			//设置字体为红色 
	while(1)//初始化OV7725_OV7670
	{
		if(OV7725_Init()==0)
		{
			sensor=OV7725;
			LCD_ShowString(30,210,200,16,16,"fuck dog de ov7725 ");
			while(1)
			{
				key=KEY_Scan(0);//你他妈的键盘扫描，非得弄的如此复杂
				if(key==KEY0_PRES)
				{
					OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGA模式输出
					break;
				}else if(key==KEY1_PRES)
				{
					OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGA模式输出
					break;
				}
				i++;
				if(i==100)LCD_ShowString(30,230,210,16,16,"KEY0:QVGA  KEY1:VGA"); //闪烁显示提示信息
				if(i==200)
				{	
					LCD_Fill(30,230,210,250+16,WHITE);
					i=0; 
				}
				delay_ms(5);
			}				
			OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;
			break;
		}else if(OV7670_Init()==0)
		{
			sensor=OV7670;
			LCD_ShowString(30,210,200,16,16,"OV7670 Init OK       ");
			delay_ms(1500);	 	   
			OV7670_Light_Mode(lightmode);
			OV7670_Color_Saturation(saturation);
			OV7670_Brightness(brightness);
			OV7670_Contrast(contrast);
			OV7670_Special_Effects(effect);
			OV7670_Window_Set(12,176,240,320);//设置窗口
			OV7670_CS=0;
			break;
		}else
		{
			LCD_ShowString(30,210,200,16,16,"OV7725_OV7670 Error!!");
			delay_ms(200);
			LCD_Fill(30,210,239,246,WHITE);
			delay_ms(200);
		}
	}

	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能外部中断8,捕获帧中断			
	LCD_Clear(BLACK);						 	 
 	while(1)
	{	
		key=KEY_Scan(0);//不支持连按
		if(key)
		{
			tm=20;
			switch(key)
			{				    
				case KEY0_PRES:	//灯光模式Light Mode
					lightmode++;
					if(sensor==OV7725)
					{
						if(lightmode>5)lightmode=0;
						OV7725_Light_Mode(lightmode);
					}else if(sensor==OV7670)
					{
						if(lightmode>4)lightmode=0;
						OV7670_Light_Mode(lightmode);
					}
					sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
					break;
				case KEY1_PRES:	//饱和度Saturation
					saturation++;
					if(sensor==OV7725)
					{
						if(saturation>4)saturation=-4;
						else if(saturation<-4)saturation=4;
						OV7725_Color_Saturation(saturation);
					}else if(sensor==OV7670)
					{
						if(saturation>2)saturation=-2;
						else if(saturation<-2)saturation=2;
						OV7670_Color_Saturation(saturation);
					}
					sprintf((char*)msgbuf,"Saturation:%d",saturation);
					break;
				case WKUP_PRES:	//对比度Contrast			    
					contrast++;
					if(sensor==OV7725)
					{
						if(contrast>4)contrast=-4;
						else if(contrast<-4)contrast=4;
						OV7725_Contrast(contrast);
					}else if(sensor==OV7670)
					{
						if(contrast>2)contrast=-2;
						else if(contrast<-2)contrast=2;
						OV7670_Contrast(contrast);
					}
					sprintf((char*)msgbuf,"Contrast:%d",contrast);
					break;
			}
		}	 
		if(TPAD_Scan(0))//检测到触摸按键 
		{
			effect++;
			if(effect>6)effect=0;
			if(sensor==OV7725)OV7725_Special_Effects(effect);		//设置特效
			else if(sensor==OV7670)OV7670_Special_Effects(effect);	//设置特效
	 		sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
			tm=20;
		}
		if(sensor==OV7725)OV7725_camera_refresh();		//更新显示
		else if(sensor==OV7670)OV7670_camera_refresh();	//更新显示
 		if(tm)
		{
			LCD_ShowString((lcddev.width-240)/2+30,(lcddev.height-320)/2+60,200,16,16,msgbuf);
			tm--;
		}
		i++;
		if(i>=15)//DS0闪烁.
		{
			i=0;
			LED0=!LED0;
 		}
	}	   
}
 }













