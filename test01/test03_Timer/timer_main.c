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

#define DDR			DDRG
#define LED0		PORTG0
#define LED1		PORTG1
#define LED2		PORTG2
#define PORT		PORTG
#define SW1			PORTD0
#define SW2			PORTD1

volatile int cnt = 0, st = 0, ival = 488, std_ival = 488;

ISR(INT0_vect)
{
	ival += std_ival / 5;
}

ISR(INT1_vect)
{
	ival -= std_ival / 5;
	if(ival < 50) ival = std_ival / 5;
}

ISR(TIMER0_OVF_vect)
{
	cnt++;
	if(cnt > ival)
	{
		cnt = 0;
		if(st)	
		{
			PORT &= ~_BV(LED0);
			st = 0;
		}
		else
		{
			PORT |= _BV(LED0);
			st = 1;
		}
	}
}
volatile int ocnt = 0, ost = 0, oival = 488;

ISR(TIMER0_COMP_vect)
{
	ocnt++;
	if(ocnt > oival)
	{
		ocnt = 0;
		if(ost)
		{
			PORT &= ~_BV(LED1);
			ost = 0;
		}
		else
		{
			PORT |= _BV(LED1);
			ost = 1;
		}
		
	}
	//TCNT0 = 0;		// OCR이 128로 설정되어 있기 때문에 TNCT0 = 0으로 초기화시 overflow 발생x
}
int main(void)
{
	
    /* Replace with your application code */
	//DDR |= (1<<LED0);
	//DDRG |= 0x06;
	DDR |= (1<<LED0) | (1<<LED1) | (1<<LED2);
	//DDR |= (1<<(LED0 | LED1 | LED2));		//안됨
	//DDR |= _BV(LED0 | LED1 | LED2);		//안됨
	PORT |= (1<<LED0) | (1<<LED1) | (1<<LED2);	//내부 pull-up 저항 설정
	PORT &= ~_BV(LED2);
	standBy();
	PORT &= ~_BV(LED0);
	
	
	TIMSK |= 0x01;				// Timer Overflow Interrupt Enable 0번째 == TOIE0
    TCCR0 |= 0x04;				// 분주비 1024 ==> 64로 변경
	
	OCR0 = 128;					// OCR을 128로 설정
	TIMSK |= 0x02;				// 비교일치 인터럽트 활성화
	
	EIMSK |= 0x03;				// external interrupt INT0,INT1 사용
	EICRA |= 0x0f;				// rising edge에서 인터럽트 발생 falling으로 만들려면 EICRA = (EICRA & ~0xf0) | 0x0a;
	
	sei();
	while (1) 
    {
    }
}

