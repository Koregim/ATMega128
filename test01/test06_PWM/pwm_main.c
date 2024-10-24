/*
 * test06_PWM.c
 *
 * Created: 2024-10-23 오전 9:46:10
 * Author : user
 */ 
#define		F_CPU	16000000
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/delay.h>
#include <avr/interrupt.h>

#define		LED_R	PORTG0
#define		LED_G	PORTG1
#define		LED_Y	PORTG2
#define		buzz	PORTG4
#define		LED_W	PORTB4
#define		DDR		DDRG
#define		PORT	PORTG

int period = 10;
int Duty_Rate = 50;

int led_y_st = 0;
int cnt = 0, cnt_ref = 0;
int pwm_val;
ISR (TIMER0_COMP_vect)
{
	//TCNT0 = 0;
	//if(++cnt > cnt_ref)
	//{
		//if(led_y_st)  {	PORT |= (1<<LED_Y);		led_y_st = 0; }
		//else		  {	PORT &= ~(1<<LED_Y);	led_y_st = 1; }
		//cnt = 0;
	//}
	
}
//ISR (TIMER0_COMP_vect)
//{
	//TCNT0 = 0;
	//pwm_val++;
	//if(led_y_st)  {	PORT |= (1<<LED_Y);		led_y_st = 0; }
	//else		  {	PORT &= ~(1<<LED_Y);	led_y_st = 1; }
//}
//ISR (INT0_vect)
//{
	//period -= 10;
	//if(period < 0)	period = 500;
//}
//ISR (INT1_vect)
//{
	//Duty_Rate += 10;
	//if(Duty_Rate > 90)	Duty_Rate = 10;
//}
//ISR (INT2_vect)
//{
	////cnt_ref--;
	////if(cnt_ref < 0) cnt_ref = 50;	//RED_Y의 깜빡이 주기 조절
	//OCR0 += 10;						//HW PWM 조절
//}
ISR (INT0_vect)
{
	OCR3A += 35;
}
ISR (INT1_vect)
{
	OCR3B += 29;
}
ISR(INT2_vect)
{
	OCR3C += 19;
}
//ISR(TIMER3_COMPA_vect)
//{
	//
//}
//ISR(TIMER3_COMPB_vect)
//{
	//
//}
//ISR(TIMER3_COMPC_vect)
//{
	//
//}
int main(void)
{
	DDR |= (1 << LED_R) | (1 << LED_G) | (1 << LED_Y);	// == 0x07;
	DDRB |= 0x10;
	DDRE |= 0x38;	
    /* Replace with your application code */

	TCCR3A |= 0xab;
	TCCR3B |= 0x0c;
	//ETIMSK |= 0x1a;
	OCR3A = 30;
	OCR3B = 15;
	OCR3C = 8;
	
	TIMSK = (1	<< OCIE0);			//	6.4us
	//TCCR0 |= 0x07;					//	1024 (분주기)
	//TCCR0 |= (1 << WGM00) | (1 << WGM01); //	11 : 고속 PWM
	//TCCR0 |= (1 << COM01);				// COM01 COM00(고속PWM 파형 조절) 10 비반전모드, 11반전 모드
	TCCR0 |= 0x6f;	
	OCR0 = 156;						//	~ 10ms
	
	EIMSK |= 0x07;
	//EICRA |= 0x3f;						//rising edge
	EICRA |= (EICRA & ~0xc0) | 0x2a;		//falling edge
	//PORT |= (1 << LED_R);
	
	sei();
	
    while (1) 
    {
		//_delay_ms(200);
		//PORT |= PORT << 1;
		//PORT &= ~(PORT >> 1);
		//if(PORT >= 0x08)
		//{
			//PORT &= ~PORT;
			//PORT |= 1<<LED_R;
		//}
		//PORT |= (1 << LED_R) | (1 << LED_G);
		//
		//_delay_ms(period * Duty_Rate / 100);
		//
		//PORT &= ~((1 << LED_R) | (1 << LED_G));
		//
		//_delay_ms(period * (100 - Duty_Rate) / 100);
    }
}

