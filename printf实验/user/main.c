#include "stm32f10x.h"
#include <stdio.h>
#include <usart.h>
#include <delay.h>
#include <sys.h>

int main()
{
	delay_init(72);
	uart_init(72,115200);
	while(1)
	{
	printf("sdasdasx\n");
	delay_ms(30);
	}
	
}
 
