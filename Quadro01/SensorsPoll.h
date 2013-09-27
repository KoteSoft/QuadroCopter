/*
 * SensorsPoll.h
 *
 * Created: 12.06.2013 19:22:34
 *  Author: Слава
 */ 


#ifndef SENSORSPOLL_H_
#define SENSORSPOLL_H_

#include "BMP085.h"
#include "ADXL345.h"
#include "HMC5883L.h"
#include "L3G4200D.h"
#include <avr/sfr_defs.h>
#include "GlobalConstants.h"
#include <math.h>
#include "Filters.h"
#include "MeasureStack.h"

uint8_t SensorsInit();
uint8_t SensorsPoll();
void Angular_Calc();
double_t Heading_displacement_calc(double_t Current, double_t Target);

#endif /* SENSORSPOLL_H_ */