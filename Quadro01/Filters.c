/*
 * Filters.c
 *
 * Created: 03.07.2013 17:51:41
 *  Author: Слава
 */ 

#include "Filters.h"


double_t LowPassFilter(double_t new_data, double_t previous_value, double_t factor)
{
	return (new_data * factor) + (previous_value * (1.0 - factor));
}

double_t HighPassFilter(double_t new_data, double_t previous_value, double_t factor)
{
	return new_data - LowPassFilter(new_data, previous_value, factor);
}

double_t ComplemetaryFilter(double_t alpha, double_t beta, double_t factor)
{
	return factor * alpha + (1.0 - factor) * beta;
}