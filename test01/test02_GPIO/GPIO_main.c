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
volatile int ival = 200;
ISR(INT0_vect)					// slower
{
	ival += 500;
}
ISR(INT1_vect)					// faster
{
	ival -= 50;
	if(ival < 10)
		ival = 20;
}
void standBy()					// PG4 pin으로 프로그램 시작 스위치 연결
{
	DDRG &= ~0x10;				// G Port 5 Bit : 입력
		
	//PORTG |= 0x10;				// (== PORTG = _BV(LED1) == (1<<LED1);  시작되면 LED ON) 
	// ==> PORTG4가 LED에서 프로그램 스위치로 변경
	PORTG |= 0x10;				// PG4 : Pull-up 설정
	
	
	
	// PORT의 값은 N/A이므로 초기값을 검증 한 후 들어가는 것이 좋음
	while((PING & 0x10) == 0);					// PING0의 초기값이 1(open)임을 가정
	
	while(1)
	{
		if((PING & 0x10) == 0)	break;			// 버튼이 눌려지지 않은 상태 = 1
	}
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
	EIMSK = 03;
	// Create register
	EICRA = 0x0A;
	
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

