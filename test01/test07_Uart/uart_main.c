/*
 * test07_Uart.c
 *
 * Created: 2024-10-25 오전 9:41:34
 * Author : user
 */ 
#define		F_CPU	16000000L
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
void UART0_init()			//setting	9600 N 8 1
{
	UCSR0A |= 0x02; // 1<<u2X0 (2배속 설정
	UBRR0L = 207; // 0xcf;	Baud rate : 9600
	UBRR0H = 0;
	UCSR0C &= ~((1<<UPM01) | (1<<UPM00)); // Parity : 00 None 10:even 11:odd
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);	//Data bits = 011 : 8(default)
	UCSR0C &= ~(1<<USBS0);					//stoop bit를 1비트로 설정: 0, 2비트로 설정 : 1
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
}

char* str = "Button Pushed!!!\r\n";
char buf[256];
int n = 0;
void UART0_putc(char c)
{
	while(1)
	{
		//while(!(UCSR0A & (1<<UDRE0))); // 입출력 버퍼가 준비될 때 까지 대기
		if(UCSR0A & (1<<UDRE0)) break; // 입출력 버퍼가 준비될 때 까지 대기
	}
	UDR0 = c;
}
void UART0_puts(char * str)
{
	while(*str)	UART0_putc(*str++);
}

ISR (INT0_vect)
{
	sprintf(buf, "Button Pushed %d times. \r\n", n++);
	
	UART0_puts(buf);
}

int main(void)
{
    /* Replace with your application code */
	// Uart resgiger setting 1 - baud rate : 9600
	//UCSR0A |= 0x02; // 1<<u2X0 (2배속 설정
	//UBRR0L = 207; // 0xcf;	Baud rate : 9600
	//UBRR0H = 0;
	//UCSR0C &= ~((1<<UPM01) | (1<<UPM00)); // Parity : 00 None 10:even 11:odd
	//UCSR0B &= ~(1<<UCSZ02);
	//UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);	//Data bits = 011 : 8(default)
	//UCSR0C &= ~(1<<USBS0);					//stoop bit를 1비트로 설정: 0, 2비트로 설정 : 1
	//UCSR0B |= (1<<RXEN0) | (1<<TXEN0);		==> 함수로 호출
	UART0_init();
	UART0_puts("\033[2J");	//screen clear ESC[2J
	UART0_puts("\033[0;0H");	//Move corsor to home ESC[X;YH 커서 위치 좌표
	// ASCII Code 에서의 esc : 27, 8진수(Octal)로 27 : \033
	EIMSK |= 0x01;
	EICRA |= 0x03;
	
	sei();
	
    while (1) 
    {
		
    }
}

