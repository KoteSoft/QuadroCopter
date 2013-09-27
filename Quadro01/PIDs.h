/*
 * PIDs.h
 *
 * Created: 12.06.2013 17:48:12
 *  Author: Слава
 */ 


#ifndef PIDS_H_
#define PIDS_H_

#include "GlobalConstants.h"
#include "ParamsDef.h"
#include "Params.h"
#include "SensorsPoll.h"

void RunPIDs();

extern double_t motors_sum[MOTORS_COUNT];

#endif /* PIDS_H_ */