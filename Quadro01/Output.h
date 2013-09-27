/*
 * Output.h
 *
 * Created: 28.06.2013 16:04:41
 *  Author: Слава
 */ 


#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdint.h>
#include "main.h"

typedef enum
{
	LED_RED		= 1,
	LED_BLUE	= 2,
	LED_GREEN	= 3	
} LED_List;

void LED_On(uint8_t LED);
void LED_Off(uint8_t LED);
void Sound_On();
void Sound_Off();

#endif /* OUTPUT_H_ */