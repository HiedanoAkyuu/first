#include "sys.h"
#include "ov7725.h"
#include "ov7725config.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"	
		    			    
//初始化OV7725
//返回0:成功
//返回其他值:错误代码
u8 OV7725_Init(void)
{
	u16 i=0;
	u16 reg=0;
	//设置IO
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);//使能相关端口时钟
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8; 	//PA8 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
		
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4);	

	
	GPIO_InitStructure.GPIO_Pin  = 0xff; //PC0~7 输入 上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_6);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14|GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_14|GPIO_Pin_15);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//SWD
 
	SCCB_Init();        		//初始化SCCB 的IO口	
	if(reg!=OV7725_PID)
	{
		printf("HID:%d\r\n",reg);
		return 2;
	}   
 	//初始化 OV7725,采用QVGA分辨率(320*240)  
	for(i=0;i<sizeof(ov7725_init_reg_tb1)/sizeof(ov7725_init_reg_tb1[0]);i++)
	{								
	   	SCCB_WR_Reg(ov7725_init_reg_tb1[i][0],ov7725_init_reg_tb1[i][1]);
 	} 
  	return 0x00; 	//ok
} 
    
void OV7725_Special_Effects(u8 eft)
{
		SCCB_WR_Reg(0xa6, 0x06);//TSLB设置
		SCCB_WR_Reg(0x60, 0x80);//MANV,手动V值
		SCCB_WR_Reg(0x61, 0x80);//MANU,手动U值
}	
//设置图像输出窗口
//width:输出图像宽度,<=320
//height:输出图像高度,<=240
//mode:0，QVGA输出模式；1，VGA输出模式
//QVGA模式可视范围广但近物不是很清晰，VGA模式可视范围小近物清晰
void OV7725_Window_Set(u16 width,u16 height)
{
	u8 raw,temp;
	u16 sx,sy;
	
	sx=(320-width)/2;
	sy=(240-height)/2;
	SCCB_WR_Reg(COM7,0x46);		//设置为QVGA模式
	SCCB_WR_Reg(HSTART,0x3f); 	//水平起始位置
	SCCB_WR_Reg(HSIZE, 0x50); 	//水平尺寸
	SCCB_WR_Reg(VSTRT, 0x03); 	//垂直起始位置
	SCCB_WR_Reg(VSIZE, 0x78); 	//垂直尺寸
	SCCB_WR_Reg(HREF,  0x00);
	SCCB_WR_Reg(HOutSize,0x50);	//输出尺寸
	SCCB_WR_Reg(VOutSize,0x78); //输出尺寸

	raw=SCCB_RD_Reg(HSTART);
	temp=raw+(sx>>2);//sx高8位存在HSTART,低2位存在HREF[5:4]
	SCCB_WR_Reg(HSTART,temp);
	SCCB_WR_Reg(HSIZE,width>>2);//width高8位存在HSIZE,低2位存在HREF[1:0]
	
	raw=SCCB_RD_Reg(VSTRT);
	temp=raw+(sy>>1);//sy高8位存在VSTRT,低1位存在HREF[6]
	SCCB_WR_Reg(VSTRT,temp);
	SCCB_WR_Reg(VSIZE,height>>1);//height高8位存在VSIZE,低1位存在HREF[2]
	
	raw=SCCB_RD_Reg(HREF);
	temp=((sy&0x01)<<6)|((sx&0x03)<<4)|((height&0x01)<<2)|(width&0x03)|raw;
	SCCB_WR_Reg(HREF,temp);
	
	SCCB_WR_Reg(HOutSize,width>>2);
	SCCB_WR_Reg(VOutSize,height>>1);
	
	SCCB_RD_Reg(EXHCH);	
	temp = (raw|(width&0x03)|((height&0x01)<<2));	
	SCCB_WR_Reg(EXHCH,temp);	
}
