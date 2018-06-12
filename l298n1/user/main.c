#include <stm32f10x.h>
#include <l298n.h>
#include <delay.h>
#include <led.h>


int main()
{
	l298n_init();
	while(1)
	{
		l298n_init();
	}
	
}
