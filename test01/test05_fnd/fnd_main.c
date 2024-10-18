/*
 * test05_fnd.c
 *
 * Created: 2024-10-16 오후 3:12:27
 * Author : user
 */ 
#define F_CPU			16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__		// Delay에 변수를 넣기 위한 선언
#include <avr/delay.h>
#include <math.h>
#define		IDDR		DDRA
#define		CDDR		DDRB
#define		IPORT		PORTA
#define		CPORT		PORTB
#define		MIN			100
unsigned char img[] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xB6, 0xbe, 0xe0, 0xfe, 0xf6};
volatile int num = 0;
char data[] = {0, 0, 0, 0};
int j = 0;
volatile int state = 2, clock_st = 0,timer_st = 0, start_Tim = 0, rst = 0;

//ISR(INT0_vect)								//	stop-watch 상태 변화
//{
	//if(state == 0)			state = 1;		//	stop-watch start
	//else if(state == 1)		state = 2;		//	stop-watch stop
	//else					state = 0;		//	stop-watch reset
//}
ISR(INT0_vect)								//	
{
	if(timer_st == 0)
	{
		
		//if(state == 0)
		//{
			//state = 1;		//
		//}
		if(state == 1)
		{
			state = 2;		//
			clock_st = 0;
		}
		else if(state == 2)
		{
			state = 1;
			clock_st = 1;
			
		}
	}
	else if(timer_st > 0)
	{
		
		if(state == 0)
		{
			state = 1;		//
		}
		else if(state == 1)
		{
			start_Tim = 1;
			state = 2;
		}
		else if(state == 2)
		{
			state = 1;
		}
	}
}
ISR(INT1_vect)
{
	if(clock_st == 1 && state == 1)		num += 100;
	if(timer_st == 1 && state == 1)		num += 1;
	if(rst == 1)
	{
		rst = 0;
		num = 0;
		timer_st = 0;
		clock_st = 1;
		PORTG &= ~0x10;
	}
	
}
ISR(INT2_vect)
{
	if(clock_st == 1 && state == 1)		num -= 100;
	if(timer_st == 1 && state == 1)		num -= 1;
	if(num < 0)							num = 0;
	
	//if(data[0] == 9)	num = 6;
}
ISR(INT3_vect)
{
	if(timer_st == 0)		
	{
		timer_st = 1;
		clock_st = 0;
		state = 0;
		
	}
	else if(timer_st == 1)	timer_st = 2;
	else if(timer_st == 2)	
	{
		timer_st = 0;
		state = 2;
	}
	
}
volatile int cnt = 0, num_cnt = 0;
int dig = 0;

ISR(TIMER0_COMP_vect)
{
	TCNT0 = 0;
	cnt++;
	
	if(cnt == 3)
	{
		cnt = 0;
		if(timer_st == 0)
		{
			if(state == 2)
			{
				num_cnt++;
				if(num_cnt == 100)
				{
					num_cnt = 0;
					num++;
				}
				
			}
		}
		else if(timer_st == 2)
		{
			if(start_Tim == 1)
			{
				num_cnt++;
				if(num_cnt == 100)
				{
					num_cnt = 0;
					num--;
					rst = 1;
					if(num == 0)		
					{
						start_Tim = 0;
						PORTG |= 0x10;
					}
				}
			}
		}
		
		
		//if(timer_st == 0)
		//{
			//
		//}
		//else
		//{
			//if(state == 0)
			//{
				//digit(num);
				//num = 0;	//	0 으로 reset
			//}
			//else if(state == 1)
			//{
				//digit(num);
				//num++;		//	start
			//}
			//else
			//{
				//digit(num);
			//}	//	멈춘값 유지
		//}
		
	}
}

int digit(int num)
{
	if(num > 6000) return 0;		//	인수 유효성 체크
	
	data[3] = num % 10;
	data[2] = (num % 100) / 10;
	data[1] = (num % 1000) / 100;
	data[0] = (num / 1000);
	
	return 1;
}

ISR(TIMER2_OVF_vect)
{
	//digit(num);
	//if(num > 9999) num = 0;
	//if(j == 4) j = 0;
	//CPORT = _BV(j);			// ==1 << i;
	//if(j == 1)	IPORT = ~(img[data[j]] + 0x01);
	//else		IPORT = ~img[data[j]];
	//j++;
	
	digit(num);
	if(num > 6000) num = 0;
	
	if(data[2] == 6)	num += 40;
	if(data[0] == 6)	num = 0;
	if(data[2] == 9)	num -= 40;
	
	
	if(j == 4) j = 0;
	CPORT = _BV(j);			// ==1 << i;
	if(j == 1)	IPORT = ~(img[data[j]] + 0x01);
	else		IPORT = ~img[data[j]];
	j++;
}

int main(void)
{
	
	IDDR |= 0xff;
	CDDR |= 0x0f;
	CPORT |= 0x0f;
	IPORT |= ~0xff;
	DDRG |= 0x10;
	DDRG |= 0x03;
	//PORTG
	
	//Timer0 100ms stop-watch, timer2 : FND refresh
	TIMSK |= 0x02;				// OCIE0
	TIMSK |= (1<<TOIE2);		// = 0x40;
	TCCR0 |= 0x06;				// 분주비 256
	TCCR2 |= 0x04;				// 분주비 256, 1/16Meg * 256 * 256 = 0.004096(sec)
	OCR0 = 208;					// count : 3 으로 10ms 설정
	//CPORT &= ~0x0f;		//	digit 4자리 off
	//IPORT |= 0xff;		//	LED 8자리 off
	
	EIMSK |= 0x0f;
	EICRA |= 0xff;
	num = 0;
	//standBy();
	sei();
	
	while(1)
	{
		if(clock_st == 1 && timer_st == 0)
		{
			PORTG |= 0x01;
			PORTG &= ~0x03;
			//
		}
		else if(timer_st > 0)
		{
			PORTG |= 0x03;
		}
	}
	
	
	
	
    /* Replace with your application code */
	
	//while (1)
	//{
		//
	//}
	//int dig = 0;
    //while (1) 
    //{
		////num = 1467;
		////for (int i = 0; i < 4; i++)
		////{
			////data[i] = (num % (int)pow(10, 4 - i)) / (int)pow(10, 3 - i);
		////}
		//for(int num = 0; num < 10000; num++)
		//{
			////data[0] = num / 1000;			// num % 10000 / 1000
			////data[1] = (num % 1000) / 100;
			////data[2] = (num % 100) / 10;
			////data[3] = num % 10;				// num % 10 / 1
			//digit(num);
				//
			//if(num < 10)						dig = 3;		//count시 앞자리수 NULL 
			//else if(num >= 10 && num < 100)		dig = 2;
			//else if(num >= 100 && num < 1000)	dig = 1;
			//else if(num >= 1000)				dig = 0;
//
		////for (int i = 0; i < 4; i++)	data[i] = (num % (int)pow(10, 4 - i)) / (int)pow(10, 3 - i);
			//for (int i = dig; i < 4; i++)
			//{
				//
				//CPORT = _BV(i);			// ==1 << i;
				////for (int j = 0; j < 4; j++)
				//{
					//IPORT = ~img[data[i]];	
					//_delay_ms(5);
				//}
			//}
			////_delay_ms(10);
		//}
    //
	//}
	
}

