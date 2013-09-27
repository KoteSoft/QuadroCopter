/*
 * Radio.h
 *
 * Created: 09.08.2013 2:20:29
 *  Author: Слава
 */ 


#ifndef RADIO_H_
#define RADIO_H_

#include "Params.h"
#include <string.h>
#include "modbus/mbcrc.h"

#define BUFFER_SIZE 17

void Byte_Recive(uint8_t Byte);

#endif /* RADIO_H_ */