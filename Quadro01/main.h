/*
 * main.h
 *
 * Created: 25.06.2013 22:49:15
 *  Author: Слава
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <limits.h>
#include <stdint.h>

#include "modbus/mb.h"
#include "modbus/mbport.h"


#include "Motors.h"
#include "SensorsPoll.h"
#include "Params.h"
#include "Timer0.h"
#include "Output.h"

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 99

#define REG_HOLDING_START 1200
#define REG_HOLDING_NREGS 299

#define REG_COILS_START     100
#define REG_COILS_SIZE      16

#define UART_BAUD_RATE          9600
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) \
( ( F_OSC ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

static USHORT   usRegInputStart = REG_INPUT_START;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldingStart = REG_HOLDING_START;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

extern unsigned char ucRegCoilsBuf[REG_COILS_SIZE / 8];

#endif /* MAIN_H_ */