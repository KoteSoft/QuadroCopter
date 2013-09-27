/*
 * BMP085.h
 *
 * Created: 26.06.2013 22:52:22
 *  Author: Слава
 */ 


#ifndef BMP085_H_
#define BMP085_H_

#include "i2c.h"
#include <util/delay.h>
#include "Params.h"
#include "main.h"
#include <math.h>

#define BMP085_ADDR 0b11101110
#define oss 3	//3

uint8_t BMP085_GetUT();
uint8_t BMP085_GetUP();
uint8_t BMP085_StartMeasUP();
uint8_t BMP085_StartMeasUT();
uint8_t BMP085_Get_Coef();

#endif /* BMP085_H_ */