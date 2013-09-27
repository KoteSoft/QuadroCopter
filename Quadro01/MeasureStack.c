/*
 * MeasureStack.c
 *
 * Created: 05.07.2013 21:58:59
 *  Author: Слава
 */ 

#include "MeasureStack.h"

uint8_t peak;
/*
double_t kx_sum[4];
double_t ky_sum[4];
double_t kz_sum[4];

double_t wx_sum[4];
double_t wy_sum[4];
double_t wz_sum[4];
*/
double_t MainStack[STACKS_COUNT];

double_t AVG_Calc(uint8_t stack)
{
	double_t z = MainStack[stack] / 64.0;
	MainStack[stack] = 0.0;
	return z;
}

void Measure_Push(double_t val, uint8_t stack)
{
	MainStack[stack] += val;
}