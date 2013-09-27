/*
 * GlobalConstants.h
 *
 * Created: 12.06.2013 18:04:44
 *  Author: Слава
 */ 

#ifndef GLOBALCONSTANTS_H_
#define GLOBALCONSTANTS_H_

#include <stdint.h>
#include "ParamsDef.h"

extern const double g;		//Ускорение свободного падения (м/с^2)
extern const double_t Step;	//Шаг квантования по времени (с)
extern const uint16_t Alarm_Long;
extern const double_t Pa2mmHg;	//1 Па = 0.00750061683 мм рт.ст.
extern const double_t D4_offset;//падение на диоде D1 0.76v

typedef enum 
{ 
	CHANNELS_COUNT = 8
}	channels_of_PWM;

typedef enum 
{
    REAR_RIGHT  =   0,
	REAR_LEFT   =   1,
	FRONT_LEFT  =   2,
    FRONT_RIGHT =   3,    
    MOTORS_COUNT
} motors_t;

typedef enum
{
	RX_MODBUS = 1,
	RX_RADIO  = 2,	
} communication_modes;


#endif /* GLOBALCONSTANTS_H_ */