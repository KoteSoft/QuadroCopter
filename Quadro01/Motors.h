/*
 * Motors.h
 *
 * Created: 12.06.2013 18:09:34
 *  Author: Слава
 */ 


#ifndef MOTORS_H_
#define MOTORS_H_

#include "ParamsDef.h"
#include "Params.h"
#include "GlobalConstants.h"
#include <avr/interrupt.h>


#define LOW 16000UL
#define HIGH 32000UL

void SetMotors(double_t* motors);	//сатурация моторов

extern volatile uint16_t counter[CHANNELS_COUNT];

#endif /* MOTORS_H_ */