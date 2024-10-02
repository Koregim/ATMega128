#include <avr/io.h>				//avr 기본 헤더
#include <util/delay.h>
#define F_CPU 10000000L
//LED Port : PG4


int main(void)
{
    /* Replace with your application code */
	DDRG |= 0x10;			// = xxx1 0000 : 1 = 출력 0 = 입력, |= xxx1 xxxx
    while (1) 
    {
		PORTG |= 0x01;
		_delay_ms(500);
		PORTG = 0x00;
		_delay_ms(500);
    }
}

