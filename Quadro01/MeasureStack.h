/*
 * MeasureStack.h
 *
 * Created: 05.07.2013 21:58:38
 *  Author: Слава
 */ 


#ifndef MEASURESTACK_H_
#define MEASURESTACK_H_

#include "ParamsDef.h"

uint8_t peak;

typedef enum
{
	KX = 0,
	KY = 1,
	KZ = 2,
	WX = 3,
	WY = 4,
	WZ = 5,
	STACKS_COUNT,
	
	STACK_SIZE = 64
}	MEASURESTACK_enum;

extern double_t MainStack[STACKS_COUNT];

double_t AVG_Calc(uint8_t array);
void Measure_Push(double_t val, uint8_t stack);

#endif /* MEASURESTACK_H_ */