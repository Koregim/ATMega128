/*
 * test04_TImer16.c
 *
 * Created: 2024-10-16 오전 11:38:09
 * Author : user
 */ 

#define			F_CPU =	16000000UL	//default 값은 1MHz delay에서 필요
#include <avr/io.h>
#include <avr/interrupt.h>
#define			DDR		DDRG
#define			PORT	PORTG
#define			LED0	PORTG0
#define			LED1	PORTG1
#define			LED2	PORTG2

volatile int st1 = 0, st2 = 0;;
ISR(TIMER1_OVF_vect)
{
	if(st1)
	{
		PORT |= _BV(LED2);
		st1 = 0;
	}
	else
	{
		PORT &= ~_BV(LED2);
		st1 = 1;
	}
}
ISR(TIMER3_OVF_vect)
{
	if(st2)
	{
		PORT |= _BV(LED1);
		st2 = 0;
	}
	else
	{
		PORT &= ~_BV(LED1);
		st2 = 1;
	}
}

int main(void)
{
    /* Replace with your application code */
	DDR |= (1<<LED0) | (1<<LED1) | (1<<LED2);	// LED포트 3개 활성화
	//PORT |= (1<<LED0) | (1<<LED1) | (1<<LED2);	// 내부 풀업저항 설정
	//standBy();
	//ETIMSK |= 0x04;		//	|= (1<<TOIE3) | (1<<OCIE3A); 도 가능 (이름 정확히 알아야만)
	ETIMSK |= _BV(TOIE3);
	TIMSK |= _BV(TOIE1);
	TCCR1B |= 0x03;
	TCCR3B |= 0x03;		//	1/(16MHz) * 1024 * 65536 대략 4초(interrupt주기)므로 TCCR3B로 분주기 64 설정(대략 0.26초)
						//	TCCR1B, TCCR3B에만 분주기 설정하는 비트가 존재 
	
	sei();
    while (1) 
    {
    }
}

