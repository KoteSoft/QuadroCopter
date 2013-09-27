/*
 * HMC5883L.h
 *
 * Created: 27.06.2013 18:14:56
 *  Author: Слава
 */ 


#ifndef HMC5883L_H_
#define HMC5883L_H_

#include "i2c.h"
#include <util/delay.h>
#include "Params.h"
#include "main.h"
#include <math.h>

#define HMC5883L_ADDR 0x3C

uint8_t HMC5883L_Poll();
uint8_t HMC5883L_Init();
uint8_t HMC5883L_Test(uint8_t mode);
uint8_t Course_Calc();

#endif /* HMC5883L_H_ */