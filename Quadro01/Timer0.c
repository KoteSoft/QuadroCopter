/*
 * Timer0.c
 *
 * Created: 30.06.2013 17:21:34
 *  Author: Слава
 */ 
#include "Timer0.h"

static uint8_t BUSY = 0;

static uint16_t AlarmTimer;

/*Прерывание, возникающее каждые 0,004096сек.*/
ISR (TIMER0_OVF_vect, ISR_NOBLOCK)
{
	if (!BUSY)
	{
		BUSY = 1;
		SensorsPoll();
		if(bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 4))	//Автоматическое управление и инклинометр
		{
			RunPIDs();		
		}
		Angular_Calc();
		BUSY = 0;
	}
	else
	{
		LED_On(LED_RED);
	}
	
	AlarmTimer++;
	if (AlarmTimer>Alarm_Long * 2)
	{
		AlarmTimer=0;
	}
	
	if (AlarmTimer>Alarm_Long && bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_SOUND], 1))
	{
		usRegHoldingBuf[MB_OFFSET+MB_SOUND] |= 1;
	}
	else
	{
		usRegHoldingBuf[MB_OFFSET+MB_SOUND] &= ~1;
	}
}