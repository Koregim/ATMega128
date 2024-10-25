/*
 * MyLib.c
 *
 * Created: 2024-10-11 오후 2:38:21
 *  Author: user
 */ 
#include <avr/io.h>
int testBit(char pin, char mask)					// PINx 레지스터 값의 mask bit가 0인지 1인지 판별
{
	if((pin & mask) != 0) return 1;
	else return 0;
}
void standBy()					// PG4 pin으로 프로그램 시작 스위치 연결
{
	DDRG &= ~0x10;				// G Port 5 Bit : 입력
	
	//PORTG |= 0x10;				// (== PORTG = _BV(LED1) == (1<<LED1);  시작되면 LED ON)
	// ==> PORTG4가 LED에서 프로그램 스위치로 변경
	PORTG |= 0x10;				// PG4 : Pull-up 설정
	// PORT의 값은 N/A이므로 초기값을 검증 한 후 들어가는 것이 좋음
	//while((PING & 0x10) == 0);					// PING0의 초기값이 1(open)임을 가정
	while(!testBit(PING, 0x10));
	
	while(testBit(PING, 0x10));					// 버튼이 눌려지지지 않은 상태 = 1
}