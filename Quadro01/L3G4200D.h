/*
 * L3G4200D.h
 *
 * Created: 29.06.2013 15:29:07
 *  Author: Слава
 */ 


#ifndef L3G4200D_H_
#define L3G4200D_H_

#include "i2c.h"
#include <util/delay.h>
#include "Params.h"
#include "main.h"
#include <math.h>
#include "MeasureStack.h"

#define L3G4200D_ADDR 0b11010010
#define L3G4200D_SENSITIVITY 0.07	//0.00875

//CTRL_REG1
#define DR1 7
#define DR0 6
#define BW1 5
#define BW0 4
#define PD  3
#define Zen 2
#define Yen 1
#define Xen 0

//CTRL_REG2
#define HPM1  5
#define HPM0  4
#define HPCF3 3
#define HPCF2 2
#define HPCF1 1
#define HPCF0 0

//CTRL_REG4
#define BDU 7
#define BLE 6
#define FS1 5
#define FS0 4
#define ST1 2
#define ST0 1
#define SIM 0

//CTRL_REG5
#define BOOT		7
#define FIFO_EN		6
#define HPen		4
#define INT1_Sel1	3
#define INT1_Sel0	2
#define Out_Sel1	1
#define Out_Sel0	0

//CTRL_REG1
#define FM2		7
#define FM1		6
#define FM0		5
#define WTM4	4
#define WTM3	3
#define WTM2	2
#define WTM1	1
#define WTM0	0

uint8_t L3G4200D_Poll();
uint8_t L3G4200D_Init();
uint8_t L3G4200D_Test(uint8_t mode);

#endif /* L3G4200D_H_ */