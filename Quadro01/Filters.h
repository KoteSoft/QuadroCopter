/*
 * Filters.h
 *
 * Created: 03.07.2013 17:51:01
 *  Author: Слава
 */ 


#ifndef FILTERS_H_
#define FILTERS_H_

#include "ParamsDef.h"

double_t LowPassFilter(double_t new_data, double_t previous_value, double_t factor);
double_t HighPassFilter(double_t new_data, double_t previous_value, double_t factor);
double_t ComplemetaryFilter(double_t alpha, double_t beta, double_t factor);

#endif /* FILTERS_H_ */