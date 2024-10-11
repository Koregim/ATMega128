/*
 * test02_GPIO.c
 *
 * Created: 2024-10-10 오전 9:36:06
 * Author : user
 */ 

#define F_CPU 16000000UL		// 설정하지 않으면 default 값은 1MHz
#include <avr/io.h>				//avr 기본 헤더
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__		// Delay에 변수를 넣기 위한 선언
#include <util/delay.h>
//input 담당 레지스터 = PIN, output 담당 레지스터 PORT
#define LED1 PORTG1				// (1<<0) LED
//#define SW2 PORTG2				// (1<<1)
//#define SW3 PORTG3				// (1<<2) (<< == shift left)
#define SW2 PORTD0				// 인터럽트 핀에 할당
#define SW3 PORTD1				// 인터럽트 핀에 할당

#define SW1 PORTG4				// 스탠바이 스위치
volatile int ival = 400;		// interrupt를 위한 변수 설정법
ISR(INT0_vect)					// slower
{
	ival += 500;
}
ISR(INT1_vect)					// faster
{
	ival -= 80;
	if(ival < 10)
		ival = 20;
}


int main(void)
{
	
    /* Replace with your application code */

	DDRG &= ~0x0C;				// 0000 1100 => 1111 0011 => xxxx 00xx입력 마스크 설정
	DDRG |= 0x03;				// xxxx xx11 출력 마스크
	PORTG |= 0x0C;				// G2~G3 Pull-up 저항
	PORTG |= _BV(LED1);			// (1<<LED1); // 0x10;
	standBy();
	PORTG &= ~_BV(LED1);
	// Mask Register
	EIMSK |= 0x03;				// INT0, INT1 활성화를위해 0000 0011 ==> 기존 인터럽트 값 유지를 위해 |=로 xxxx xx11
	// Create register
	//EICRA |= 0x0f;				// INT0과 INT1의 interrupt timing을 rising edge로 설정하기 위해 xxxx 1111로 설정
								// falling edge로 설정하기 위해 EICRA |= 0x0A로 설정한다면 xxxx 1x1x가 되어버림
	//EICRA |= 0x0a;	// xxxx 1x1x
	//EICRA &= ~0x05;	// xxxx x0x0 추가
	//==> 한줄로
	EICRA = (EICRA & 0xf0) | 0x0a;
	
	sei();
    while (1) 
    {
		//if((PING & (1<<SW2)) == 0)			// 01, slower
		//{
			//ival += 500;
		//}
		//else if((PING & (1<<SW3)) == 0)			// 02, faster
		//{
			//ival -= 50;
			//if(ival < 10)
				//ival = 50;
		//}										// 인터럽트로 구현
		PORTG |= _BV(LED1);				//LED On
		_delay_ms(ival);
		PORTG &= ~(_BV(LED1));			//LED Off
		_delay_ms(ival);
    }
}

