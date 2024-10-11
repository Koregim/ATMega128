/*
 * test03_Timer.c
 *
 * Created: 2024-10-11 오후 2:33:09
 * Author : user
 */ 

#define F_CPU 16000000UL		// 설정하지 않으면 default 값은 1MHz
#include <avr/io.h>				//avr 기본 헤더
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__		// Delay에 변수를 넣기 위한 선언
#include <util/delay.h>

#define DDR		DDRG
#define LED		PORTG1
#define PORT	PORTG
#define SW1		PORTD0
#define SW2		PORTD1

volatile int cnt = 0, st = 0, ival = 488;

ISR(INT0_vect)
{
	ival += 98;
}

ISR(INT1_vect)
{
	ival -= 98;
	if(ival < 50) ival = 96;
}

ISR(TIMER0_OVF_vect)
{
	cnt++;
	if(cnt > ival)
	{
		cnt = 0;
		if(st)	
		{
			PORT &= ~_BV(LED);
			st = 0;
		}
		else
		{
			PORT |= _BV(LED);
			st = 1;
		}
	}
}
int main(void)
{
	
    /* Replace with your application code */
	DDR |= (1<<LED);
	PORT |= (1<<LED);
	standBy();
	PORT &= ~_BV(LED);
	
	TIMSK |= 0x01;				// Timer Overflow Interrupt Enable 0번째 == TOIE0
    TCCR0 |= 0x04;				// 분주비 1024 ==> 64로 변경
	EIMSK |= 0x03;
	EICRA |= 0x0f;
	
	sei();
	while (1) 
    {
    }
}

