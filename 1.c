#include<reg52.h>
#include<string.h>
#include<stdio.h>
#include<intrins.h>

#define led P2

unsigned char b;
unsigned char flag;
unsigned char d[10];
unsigned char i=0;

void init();
void receive(unsigned char);
void delay(unsigned int);

void main()
{
	unsigned int c;
	init();
	b=0xfe;
	while(1)
	{
		led=b;
		delay(500);
		b=_crol_(b, 1);
		if (flag==1&&i<10)
		{
			flag=0;
		}
		for (i=0;i<10;i++)
		{
			if (d[i]=='\n')
			{	
				c=strlen(d);
				ES=0;
				TI=1; 
				printf("%d", c);
				while (!TI);//*********
				TI=0;
			}
		}
	}
}
	
void init()
{
	TMOD=0x20;
	TH1=0xf3;
	TL1=0xf3;
	PCON=0x80;
	TR1=1;//定时器1开关
	SCON=0x50;
	EA=1;//总开关
	ES=1;//打开串口中断
}

void aa() interrupt 4
{
	if (RI)
	{
		RI=0;
		d[i]=SBUF;
	}
	flag=1;
	i++;
}
void delay(unsigned int a)
{
	unsigned int i, j;
	for (i=a;i>0;i--)
		for(j=110;j>0;j--);
}

		
