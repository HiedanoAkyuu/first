#include<key.h>

#include<sys.h>

#include <delay.h>
//按键初始化函数
void KEY_Init(void) //IO 初始化
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE); //打开GPIOA和GPIOE时钟
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//GPIOE.3~4
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
GPIO_Init(GPIOE, &GPIO_InitStructure); //初始化GPIOE3,4
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //将WK_P连上GPIOA.0
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成下拉输入
GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA.0
}	
u8 KEY_Scan(u8 mode)
{
static u8 key_up=1;//表示最开始时按键是松开的
if(mode)key_up=1; //如果mode==1,支持连按
if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
{
delay_ms(10);//去抖动
key_up=0;
if(KEY0==0)return KEY0_PRES;
else if(KEY1==0)return KEY1_PRES;
else if(WK_UP==1)return WKUP_PRES;
}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1;
return 0;//返回无按键按下，k0,k1低电平有效，keyup高电平有效
}
