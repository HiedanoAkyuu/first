#include "sys.h"
#include "ov7670.h"
#include "ov7670cfg.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"
#include  "DCMI.h"	




/********************************************************************************/
//初始化OV7670
//返回0:成功
//返回其他值:错误代码

u16 OV7670_Init(void)
{
  GPIO_InitTypeDef  GPIO_Structure;
	u16 reg, i=0;
	
	/*** GPIO 时钟配置 ***/
	__HAL_RCC_GPIOA_CLK_ENABLE();              //打开 GPIOA时钟
	__HAL_RCC_GPIOG_CLK_ENABLE();              //打开GPIOG 时钟

	//HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSE,RCC_MCODIV_1); //MCO1时钟HSE(25MHZ);
	//HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSE,RCC_MCODIV_3); //MCO1时钟HSE(25M)/3=8.3333MHZ;
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSI,RCC_MCODIV_2); //MCO1时钟HSI(16M)/2=8MHZ;
	/*** GPIO  配置 ***/
	GPIO_Structure.Pin = GPIO_PIN_2|GPIO_PIN_3;     //PG2->RST; PG3->PWDN;
	GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;      //推挽输出
	GPIO_Structure.Speed = GPIO_SPEED_FREQ_HIGH;    //高速(108M/2)
	GPIO_Structure.Pull = GPIO_PULLUP;              //上拉
	HAL_GPIO_Init(GPIOG, &GPIO_Structure);          //GPIO 初始化

	GPIO_Structure.Pin = GPIO_PIN_8;                //PA8->MCO1;
	GPIO_Structure.Mode = GPIO_MODE_AF_PP;          //复用推挽;
	GPIO_Structure.Pull = GPIO_NOPULL;              //无上下拉
	GPIO_Structure.Alternate = GPIO_AF0_MCO;        //MCO1 时钟输出
	HAL_GPIO_Init(GPIOA, &GPIO_Structure);          //GPIO 初始化
	
	delay_ms(5);
	OV7670_RST(0);			//必须先拉低OV7670的RST脚,再上电 //复位OV7670
	OV7670_PWDN(1);	//POWER OFF
	delay_ms(20);
 	OV7670_PWDN(0);	//POWER ON
	delay_ms(5);
	OV7670_RST(1);			//结束复位
	delay_ms(10);
  SCCB_Init();        		//初始化SCCB 的IO口
 	OV7670_WR_Reg(0X12, 0x80);	//软复位OV7670
	delay_ms(50);
	reg=OV7670_RD_Reg(0X1c);	//读取厂家ID 高八位
	reg<<=8;
	reg|=OV7670_RD_Reg(0X1d);	//读取厂家ID 低八位
	if(reg!=OV7670_MID)
	{
		printf("MID:%d\r\n",reg);
		return 1;
	}
	//读取产品型号
	reg=OV7670_RD_Reg(0x0a);	//读取ID 高八位
	reg<<=8;
	reg|=OV7670_RD_Reg(0x0b);	//读取ID 低八位
	if(reg!=OV7670_HID)
	{
		printf("Camera HID Error:%04x\r\n",reg);
		return 1;
	}else printf("Camera HID:%04x\r\n",reg);
	//初始化序列    VGA 640*480(~30W)
	for(i=0;i<sizeof(ov7670_init_reg_tbl)/sizeof(ov7670_init_reg_tbl[0]);i++)
	{
			OV7670_WR_Reg(ov7670_init_reg_tbl[i][0],ov7670_init_reg_tbl[i][1]);
	}
	
//	for(i=0;i<sizeof(ov7670_init_reg_tbl2)/sizeof(ov7670_init_reg_tbl2[0]);i++)
//	{
//	  OV7670_WR_Reg(ov7670_init_reg_tbl2[i][0],ov7670_init_reg_tbl2[i][1]);
//	}
/***********************************************/
		delay_us(10*1000);
		DCMI_Init();			//DCMI配置
		
		DCMI_Clock_Set(0,0,1);
/***********************************************/
		OV7670_SetWindow_CIF(0,14, 352,290);      //CIF 输出窗口设置  352x(288+2)

//		OV7670_SetWindow(10, 176, 320,240);    //图像输出大小设置  (旧, 无用)

//		DCMI_Start(); 			//启动传输
/***********************************************/
   	return reg; 	//ok
}
/********************************************************************************/
//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home

void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//默认就是设置为自动白平衡
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	OV7670_WR_Reg(0X13,reg13val);//COM8设置 
	OV7670_WR_Reg(0X01,reg01val);//AWB蓝色通道增益 
	OV7670_WR_Reg(0X02,reg02val);//AWB红色通道增益 
}
/********************************************************************************/
//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	OV7670_WR_Reg(0X4F,reg4f5054val);	//色彩矩阵系数1
	OV7670_WR_Reg(0X50,reg4f5054val);	//色彩矩阵系数2 
	OV7670_WR_Reg(0X51,0X00);			//色彩矩阵系数3  
	OV7670_WR_Reg(0X52,reg52val);		//色彩矩阵系数4 
	OV7670_WR_Reg(0X53,reg53val);		//色彩矩阵系数5 
	OV7670_WR_Reg(0X54,reg4f5054val);	//色彩矩阵系数6  
	OV7670_WR_Reg(0X58,0X9E);			//MTXS 
}
/********************************************************************************/
//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	OV7670_WR_Reg(0X55,reg55val);	//亮度调节 
}
/********************************************************************************/
//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	OV7670_WR_Reg(0X56,reg56val);	//对比度调节 
}
/********************************************************************************/
//特效设置
//0:普通模式
//1,负片
//2,黑白
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古

void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	OV7670_WR_Reg(0X3A,reg3aval);//TSLB设置 
	OV7670_WR_Reg(0X68,reg67val);//MANU,手动U值 
	OV7670_WR_Reg(0X67,reg68val);//MANV,手动V值 
}
/********************************************************************************/
//CIF 输出窗口设置            //CIF输出  352x(288+2)
//示例: OV7670_SetWindow_CIF(0x24, 0,14,352,290);

u8 OV7670_SetWindow_CIF(u16 offx,u16 offy,u16 width,u16 height) //CIF 输出窗口设置  352x(288+2)
{
	u16 start_v, stop_v, start_h, stop_h;
	u16 temp;
	
	start_v = offy;
	start_h = 174 + offx +((352 - width)*2);
	stop_v = 494 + height - 290;
	stop_h = 94 + offx;
	
	while(OV7670_WR_Reg(0x12, 0x24)); //设置输出格式RAW, CIF 352*(288+2);
	
	temp = OV7670_RD_Reg(0x03);        //读取VREF 之前的值
	temp &= 0xF0;
	temp |= (start_v&0x03)|((stop_v&0x03)<<2);
	OV7670_WR_Reg(0x03, temp); //2-2   //VREF    位[3:2]VREF结束的低两位(高八位见VSTOP[7:0])
												                     //位[1:0]VREF开始的低两位(高八位见VSTOP[7:0])
	OV7670_WR_Reg(0x19, start_v>>2); //vs_14//VSTRT， 输出格式-场频开始高八位(低二位在VREF[1:0])
	OV7670_WR_Reg(0x1A, stop_v>>2); //ve_494//VSTOP，输出格式-场频结束高八位(低二位在VREF[3:2]);

	temp = OV7670_RD_Reg(0x32);        //读取HREF 之前的设置
	temp &= 0xC0;
	temp |= (start_h&0x07)|((stop_h&0x07)<<3);
	OV7670_WR_Reg(0x32, temp); //6-6   //HREF，HREF控制
																						 //位[7:6]:HREF沿距数据输出的偏移
																						 //位[5:3]:HREF结束的低3位(高8位在HSTOP)
																						 //位[2:0]:HREF开始的低3位(高8位在HSTOP)
	OV7670_WR_Reg(0x17, start_h>>3); //hs_174//HSTART，输出格式-行频开始高八位(低三位在HREF[2:0])
	OV7670_WR_Reg(0x18, stop_h>>3); //he_94//HSTOP，输出格式-行频结束高八位(低三位在HREF[5:3])

	OV7670_WR_Reg(0x70, 0x3A); //SCALING_XSC，水平缩放系数
	OV7670_WR_Reg(0x71, 0x35); //SCALING_YSC，垂直缩放系数
	OV7670_WR_Reg(0x72, 0x11); //SCALING_DCWCTR，DCW控制
	OV7670_WR_Reg(0x73, 0xF2);        //SCALING_PC，旁路DSP缩放时钟分频控制
	OV7670_WR_Reg(0xA2, 0x03); //SCALING_PCLK_DELAY，像素始终延迟
	return  0;
}
/********************************************************************************/
//设置图像输出窗口       (旧, 无用)
//对QVGA设置

u8 OV7670_SetWindow(u16 offx,u16 offy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp;
	endx=offx+width*2;	//V*2
 	endy=offy+height*2;
	if(endy>784)endy-=784;
	temp=OV7670_RD_Reg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(offx&0X03);
	OV7670_WR_Reg(0X03,temp);				//设置Vref的start和end的最低2位
	OV7670_WR_Reg(0X19,offx>>2);			//设置Vref的start高8位
	OV7670_WR_Reg(0X1A,endx>>2);			//设置Vref的end的高8位

	temp=OV7670_RD_Reg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(offy&0X07);
	OV7670_WR_Reg(0x32, temp);        //设置HREF的start和end的低3位
	OV7670_WR_Reg(0X17,offy>>3);			//设置Href的start高8位
	OV7670_WR_Reg(0X18,endy>>3);			//设置Href的end的高8位
	return  0;
}
/********************************************************************************/
//写寄存器
//返回值:0,成功;1,失败.
 
u8 OV7670_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(OV7670_ADDR))res=1;	//写器件ID	  
	delay_us(30);
	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(30);
	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
	SCCB_Stop();	  
	return	res;
}
/********************************************************************************/	  					    
//读寄存器
//返回值:读到的寄存器值

u8 OV7670_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(OV7670_ADDR);		//写器件ID	  
	delay_us(30);	 
	SCCB_WR_Byte(reg);			//写寄存器地址	  
	delay_us(10);
	SCCB_Stop();   
	delay_us(30);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(OV7670_ADDR|0X01);	//发送读命令	  
	delay_us(30);
	val=SCCB_RD_Byte();		 	//读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
/********************************************************************************/
