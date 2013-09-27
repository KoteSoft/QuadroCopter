/*
 * Output.c
 *
 * Created: 28.06.2013 16:04:54
 *  Author: Слава
 */ 
#include "Output.h"

void LED_On(uint8_t LED)
{
	switch (LED)
	{
		case LED_RED: 
		{
			PORTB |= 0b00100000;
			break;
		}
		case LED_BLUE:
		{
			PORTB |= 0b01000000;
			break;
		}
		case LED_GREEN:
		{
			PORTB |= 0b10000000;
			break;
		}
		default:
		{
			
		}
	}
}

void LED_Off(uint8_t LED)
{
	switch (LED)
	{
		case LED_RED:
		{
			PORTB &= ~0b00100000;
			break;
		}
		case LED_BLUE:
		{
			PORTB &= ~0b01000000;
			break;
		}
		case LED_GREEN:
		{
			PORTB &= ~0b10000000;
			break;
		}
		default:
		{
			
		}
	}
}


void Sound_On()
{
	PORTB |= 0b00001000;
}

void Sound_Off()
{
	PORTB &= ~0b00001000;
}