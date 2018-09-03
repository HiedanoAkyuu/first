#include "sys.h"
#include "ov7670.h"
#include "ov7670cfg.h"	  
#include "delay.h"
#include "usart.h"			 
#include "sccb.h"
#include  "DCMI.h"	




/********************************************************************************/
//��ʼ��OV7670
//����0:�ɹ�
//��������ֵ:�������

u16 OV7670_Init(void)
{
  GPIO_InitTypeDef  GPIO_Structure;
	u16 reg, i=0;
	
	/*** GPIO ʱ������ ***/
	__HAL_RCC_GPIOA_CLK_ENABLE();              //�� GPIOAʱ��
	__HAL_RCC_GPIOG_CLK_ENABLE();              //��GPIOG ʱ��

	//HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSE,RCC_MCODIV_1); //MCO1ʱ��HSE(25MHZ);
	//HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSE,RCC_MCODIV_3); //MCO1ʱ��HSE(25M)/3=8.3333MHZ;
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCOSOURCE_HSI,RCC_MCODIV_2); //MCO1ʱ��HSI(16M)/2=8MHZ;
	/*** GPIO  ���� ***/
	GPIO_Structure.Pin = GPIO_PIN_2|GPIO_PIN_3;     //PG2->RST; PG3->PWDN;
	GPIO_Structure.Mode = GPIO_MODE_OUTPUT_PP;      //�������
	GPIO_Structure.Speed = GPIO_SPEED_FREQ_HIGH;    //����(108M/2)
	GPIO_Structure.Pull = GPIO_PULLUP;              //����
	HAL_GPIO_Init(GPIOG, &GPIO_Structure);          //GPIO ��ʼ��

	GPIO_Structure.Pin = GPIO_PIN_8;                //PA8->MCO1;
	GPIO_Structure.Mode = GPIO_MODE_AF_PP;          //��������;
	GPIO_Structure.Pull = GPIO_NOPULL;              //��������
	GPIO_Structure.Alternate = GPIO_AF0_MCO;        //MCO1 ʱ�����
	HAL_GPIO_Init(GPIOA, &GPIO_Structure);          //GPIO ��ʼ��
	
	delay_ms(5);
	OV7670_RST(0);			//����������OV7670��RST��,���ϵ� //��λOV7670
	OV7670_PWDN(1);	//POWER OFF
	delay_ms(20);
 	OV7670_PWDN(0);	//POWER ON
	delay_ms(5);
	OV7670_RST(1);			//������λ
	delay_ms(10);
  SCCB_Init();        		//��ʼ��SCCB ��IO��
 	OV7670_WR_Reg(0X12, 0x80);	//��λOV7670
	delay_ms(50);
	reg=OV7670_RD_Reg(0X1c);	//��ȡ����ID �߰�λ
	reg<<=8;
	reg|=OV7670_RD_Reg(0X1d);	//��ȡ����ID �Ͱ�λ
	if(reg!=OV7670_MID)
	{
		printf("MID:%d\r\n",reg);
		return 1;
	}
	//��ȡ��Ʒ�ͺ�
	reg=OV7670_RD_Reg(0x0a);	//��ȡID �߰�λ
	reg<<=8;
	reg|=OV7670_RD_Reg(0x0b);	//��ȡID �Ͱ�λ
	if(reg!=OV7670_HID)
	{
		printf("Camera HID Error:%04x\r\n",reg);
		return 1;
	}else printf("Camera HID:%04x\r\n",reg);
	//��ʼ������    VGA 640*480(~30W)
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
		DCMI_Init();			//DCMI����
		
		DCMI_Clock_Set(0,0,1);
/***********************************************/
		OV7670_SetWindow_CIF(0,14, 352,290);      //CIF �����������  352x(288+2)

//		OV7670_SetWindow(10, 176, 320,240);    //ͼ�������С����  (��, ����)

//		DCMI_Start(); 			//��������
/***********************************************/
   	return reg; 	//ok
}
/********************************************************************************/
//OV7670��������
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home

void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//Ĭ�Ͼ�������Ϊ�Զ���ƽ��
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
	OV7670_WR_Reg(0X13,reg13val);//COM8���� 
	OV7670_WR_Reg(0X01,reg01val);//AWB��ɫͨ������ 
	OV7670_WR_Reg(0X02,reg02val);//AWB��ɫͨ������ 
}
/********************************************************************************/
//ɫ������
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//Ĭ�Ͼ���sat=2,��������ɫ�ȵ�����
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
	OV7670_WR_Reg(0X4F,reg4f5054val);	//ɫ�ʾ���ϵ��1
	OV7670_WR_Reg(0X50,reg4f5054val);	//ɫ�ʾ���ϵ��2 
	OV7670_WR_Reg(0X51,0X00);			//ɫ�ʾ���ϵ��3  
	OV7670_WR_Reg(0X52,reg52val);		//ɫ�ʾ���ϵ��4 
	OV7670_WR_Reg(0X53,reg53val);		//ɫ�ʾ���ϵ��5 
	OV7670_WR_Reg(0X54,reg4f5054val);	//ɫ�ʾ���ϵ��6  
	OV7670_WR_Reg(0X58,0X9E);			//MTXS 
}
/********************************************************************************/
//��������
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//Ĭ�Ͼ���bright=2
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
	OV7670_WR_Reg(0X55,reg55val);	//���ȵ��� 
}
/********************************************************************************/
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,1
//4,2

void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//Ĭ�Ͼ���contrast=2
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
	OV7670_WR_Reg(0X56,reg56val);	//�Աȶȵ��� 
}
/********************************************************************************/
//��Ч����
//0:��ͨģʽ
//1,��Ƭ
//2,�ڰ�
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����

void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//Ĭ��Ϊ��ͨģʽ
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://�ڰ�
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://����
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	OV7670_WR_Reg(0X3A,reg3aval);//TSLB���� 
	OV7670_WR_Reg(0X68,reg67val);//MANU,�ֶ�Uֵ 
	OV7670_WR_Reg(0X67,reg68val);//MANV,�ֶ�Vֵ 
}
/********************************************************************************/
//CIF �����������            //CIF���  352x(288+2)
//ʾ��: OV7670_SetWindow_CIF(0x24, 0,14,352,290);

u8 OV7670_SetWindow_CIF(u16 offx,u16 offy,u16 width,u16 height) //CIF �����������  352x(288+2)
{
	u16 start_v, stop_v, start_h, stop_h;
	u16 temp;
	
	start_v = offy;
	start_h = 174 + offx +((352 - width)*2);
	stop_v = 494 + height - 290;
	stop_h = 94 + offx;
	
	while(OV7670_WR_Reg(0x12, 0x24)); //���������ʽRAW, CIF 352*(288+2);
	
	temp = OV7670_RD_Reg(0x03);        //��ȡVREF ֮ǰ��ֵ
	temp &= 0xF0;
	temp |= (start_v&0x03)|((stop_v&0x03)<<2);
	OV7670_WR_Reg(0x03, temp); //2-2   //VREF    λ[3:2]VREF�����ĵ���λ(�߰�λ��VSTOP[7:0])
												                     //λ[1:0]VREF��ʼ�ĵ���λ(�߰�λ��VSTOP[7:0])
	OV7670_WR_Reg(0x19, start_v>>2); //vs_14//VSTRT�� �����ʽ-��Ƶ��ʼ�߰�λ(�Ͷ�λ��VREF[1:0])
	OV7670_WR_Reg(0x1A, stop_v>>2); //ve_494//VSTOP�������ʽ-��Ƶ�����߰�λ(�Ͷ�λ��VREF[3:2]);

	temp = OV7670_RD_Reg(0x32);        //��ȡHREF ֮ǰ������
	temp &= 0xC0;
	temp |= (start_h&0x07)|((stop_h&0x07)<<3);
	OV7670_WR_Reg(0x32, temp); //6-6   //HREF��HREF����
																						 //λ[7:6]:HREF�ؾ����������ƫ��
																						 //λ[5:3]:HREF�����ĵ�3λ(��8λ��HSTOP)
																						 //λ[2:0]:HREF��ʼ�ĵ�3λ(��8λ��HSTOP)
	OV7670_WR_Reg(0x17, start_h>>3); //hs_174//HSTART�������ʽ-��Ƶ��ʼ�߰�λ(����λ��HREF[2:0])
	OV7670_WR_Reg(0x18, stop_h>>3); //he_94//HSTOP�������ʽ-��Ƶ�����߰�λ(����λ��HREF[5:3])

	OV7670_WR_Reg(0x70, 0x3A); //SCALING_XSC��ˮƽ����ϵ��
	OV7670_WR_Reg(0x71, 0x35); //SCALING_YSC����ֱ����ϵ��
	OV7670_WR_Reg(0x72, 0x11); //SCALING_DCWCTR��DCW����
	OV7670_WR_Reg(0x73, 0xF2);        //SCALING_PC����·DSP����ʱ�ӷ�Ƶ����
	OV7670_WR_Reg(0xA2, 0x03); //SCALING_PCLK_DELAY������ʼ���ӳ�
	return  0;
}
/********************************************************************************/
//����ͼ���������       (��, ����)
//��QVGA����

u8 OV7670_SetWindow(u16 offx,u16 offy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp;
	endx=offx+width*2;	//V*2
 	endy=offy+height*2;
	if(endy>784)endy-=784;
	temp=OV7670_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(offx&0X03);
	OV7670_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	OV7670_WR_Reg(0X19,offx>>2);			//����Vref��start��8λ
	OV7670_WR_Reg(0X1A,endx>>2);			//����Vref��end�ĸ�8λ

	temp=OV7670_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(offy&0X07);
	OV7670_WR_Reg(0x32, temp);        //����HREF��start��end�ĵ�3λ
	OV7670_WR_Reg(0X17,offy>>3);			//����Href��start��8λ
	OV7670_WR_Reg(0X18,endy>>3);			//����Href��end�ĸ�8λ
	return  0;
}
/********************************************************************************/
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��.
 
u8 OV7670_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(OV7670_ADDR))res=1;	//д����ID	  
	delay_us(30);
	if(SCCB_WR_Byte(reg))res=1;		//д�Ĵ�����ַ	  
	delay_us(30);
	if(SCCB_WR_Byte(data))res=1; 	//д����	 
	SCCB_Stop();	  
	return	res;
}
/********************************************************************************/	  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ

u8 OV7670_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(OV7670_ADDR);		//д����ID	  
	delay_us(30);	 
	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	delay_us(10);
	SCCB_Stop();   
	delay_us(30);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(OV7670_ADDR|0X01);	//���Ͷ�����	  
	delay_us(30);
	val=SCCB_RD_Byte();		 	//��ȡ����
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
/********************************************************************************/
