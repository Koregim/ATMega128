#define F_CPU 16000000UL
#include <avr/io.h>				//avr 기본 헤더
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

//LED Port : PG4
// SW Port : PG3
static int count = 0;
static int last_but;
static int del_val1 = 250, del_val2 = 250;
int check(void)
{
	char b1, b2, b3;
	b1 = PING & 0x01;
	b2 = PING & 0x02;
	b3 = PING & 0x04;
	int last_sta = 0, cur_sta = 0;
	
	if(b1 == 0)
	{
		cur_sta = 1;
		if(last_sta != cur_sta)
		{
			_delay_ms(5);
			last_sta = 1;
			count = 1;
			//if(count == 2)
			//{
				//count = 0;
			//}
			last_but = 1;
			return 1;
		}
		
	}
	else if(b2 == 0)
	{
		cur_sta = 1;
		if(last_sta != cur_sta)
		{
			_delay_ms(5);
			last_sta = 1;
			last_but = 2;
			del_val1 -= 15;
			del_val2 = 250;
			return 2;
		}
	}
	else if(b3 == 0)
	{
		cur_sta = 1;
		if(last_sta != cur_sta)
		{
			_delay_ms(5);
			last_sta = 1;
			last_but = 3;
			del_val2 += 50;
			del_val1 = 250;
			return 3;
		}
	}
	else
	{
		last_sta = cur_sta;
		cur_sta = 0;
	}
	return last_but;
	//else
	//{
		//curBut = 0;
		//lastBut = curBut;
	//}
	//else
	//{
		//curBut = 0;
		//_delay_ms(10);
	//}
}

int main(void)
{
	
    /* Replace with your application code */
	DDRG |= 0x10;		 // == DDG3 = 1 bit에 직접 접근, const(상수)
	DDRG &= ~0x07;


	//while(1)			//waiting 버튼이 눌리면 아래 while 시작
	//{
		//v = PING & 0x08;
		//if(v == 0) break;
	//}
	//
	// 1 0 == (0001 0000) ff == 1111 1111, xxx1 0000 : 1 = 출력 0 = 입력, |= xxx1 xxxx
	
    while (1) 
    {
		//if(count == 0)
			//PORTG &= ~0x10;
		if(check() == 1 && count == 1)
		{
			PORTG |= 0x10;
			_delay_ms(250);
			PORTG &= ~0x10;			// 0001 0000 => 1110 1111 => xxx0 xxxx
			_delay_ms(250);
		}
		else if(check() == 2 && count == 1)
		{
			PORTG |= 0x10;
			_delay_ms(del_val1);
			PORTG &= ~0x10;			// 0001 0000 => 1110 1111 => xxx0 xxxx
			_delay_ms(del_val1);
		}
		else if(check() == 3 && count == 1)
		{
			PORTG |= 0x10;
			_delay_ms(del_val2);
			PORTG &= ~0x10;			// 0001 0000 => 1110 1111 => xxx0 xxxx
			_delay_ms(del_val2);
		}
    }
}

