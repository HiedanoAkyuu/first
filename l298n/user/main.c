#include <stm32f10x.h>

#include <l298n.h>
#include <delay.h>
#include <led.h>
#include <sys.h>
#include <exit.h>



void l298n_roll()
{
	IN1=0;
	IN2=1;
	PBout(5)=0;
	delay_ms(100);
	PBout(5)=1;
	delay_ms(100);
}


int main()
{
	delay_init(72);
	LED_Init();
	l298n_init();
	EXTIINIT();
	
	while(1)
	{
//		PEout(5)=0;
//		delay_ms(100);
//		PEout(5)=1;
//		delay_ms(100);
	l298n_roll();
	
	}
}



