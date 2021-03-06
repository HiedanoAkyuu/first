#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"

 
/************************************************
 ALIENTEK战舰STM32开发板实验35
 摄像头OV7670 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5种光照模式	    
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义		 

unsigned char txbuf[4];

void camera_refresh(void)
{
	u8 v=140,h=105;
		u8 color_r,color_g,color_b;
	u16 k;
	u8 j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新？
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右  
		if(lcddev.id==0X1963)LCD_Set_Window((lcddev.width-240)/2,(lcddev.height-320)/2,240,320);//将显示区域设置到屏幕中央
		else if(lcddev.id==0X5510||lcddev.id==0X5310)LCD_Set_Window((lcddev.width-320)/2,(lcddev.height-240)/2,320,240);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
		for(j=0;j<240;j++)
		{
			for(k=0;k<320;k++){
				OV7670_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7670_RCK_H; 
				color<<=8;  
				OV7670_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7670_RCK_H; 
			
//				LCD->LCD_RAM=color;  
			

					color_r=color>>11;
					color_g=(color&0x07ff)>>5;
					color_b=color&0x001f;//将565分别给RGB

				color_g=(color_g<<2)*58;
				color_b=(color_b<<3)*11;
				color=(color_r+color_g+color_b+50)/100;

				if(color>100){
					LCD->LCD_RAM=0xffff;
				}
				else{
					LCD->LCD_RAM=0;//非常重要的图像二值化处理*********************

				}

			
				

				}
			}

				
		}	
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}	   



 int main(void)
 {	 

	 
	u8 key;
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;
	u8 effect=0;	 
 	u8 i=0;	    
	u8 msgbuf[15];				//消息缓存区
	u8 tm=0; 

	 	txbuf[0]=0x01;
		txbuf[1]=0x02;
		txbuf[2]=0x03;
		txbuf[3]=0x04;
	 
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200
 	usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	TPAD_Init(6);				//触摸按键初始化 
 	POINT_COLOR=RED;			//设置字体为红色 
	while(OV7670_Init())//初始化OV7670
	{
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
	    LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
	delay_ms(1500);	 	   
	OV7670_Light_Mode(lightmode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
 	OV7670_Special_Effects(effect);	 
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7670_Window_Set(12,176,240,320);	//设置窗口	  
  	OV7670_CS=0;					
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
					if(lightmode>4)lightmode=0;
					OV7670_Light_Mode(lightmode);
					sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
					break;
				case KEY1_PRES:	//饱和度Saturation
					saturation++;
					if(saturation>4)saturation=0;
					OV7670_Color_Saturation(saturation);
					sprintf((char*)msgbuf,"Saturation:%d",(signed char)saturation-2);
					break;
				case KEY2_PRES:	//亮度Brightness				 
					brightness++;
					if(brightness>4)brightness=0;
					OV7670_Brightness(brightness);
					sprintf((char*)msgbuf,"Brightness:%d",(signed char)brightness-2);
					break;
				case WKUP_PRES:	//对比度Contrast			    
					contrast++;
					if(contrast>4)contrast=0;
					OV7670_Contrast(contrast);
					sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-2);
					break;
			}
		}	 
		if(TPAD_Scan(0))//检测到触摸按键 
		{
			effect++;
			if(effect>6)effect=0;
			OV7670_Special_Effects(effect);//设置特效
	 		sprintf((char*)msgbuf,"%s",EFFECTS_TBL[effect]);
			tm=20;
		} 
		camera_refresh();//更新显示
 		if(tm)
		{
			LCD_ShowString((lcddev.width-240)/2+30,(lcddev.height-320)/2+60,200,16,16,msgbuf);
			tm--;
		}
		i++;
		if(i==15)//DS0闪烁.
		{
			i=0;
			LED0=!LED0;
 		}
	}	   
}













