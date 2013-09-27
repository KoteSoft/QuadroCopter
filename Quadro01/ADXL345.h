/*
 * ADXL345.h
 *
 * Created: 27.06.2013 13:47:55
 *  Author: Слава
 */ 


#ifndef ADXL345_H_
#define ADXL345_H_

#include "i2c.h"
#include <util/delay.h>
#include "Params.h"
#include "main.h"
#include <math.h>
#include "Filters.h"
#include "MeasureStack.h"

#define ADXL345_ADDR 0xA6

uint8_t ADXL345_Poll();
uint8_t ADXL345_Init();

#endif /* ADXL345_H_ */