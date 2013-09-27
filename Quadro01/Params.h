/*
 * Params.h
 *
 * Created: 12.06.2013 16:16:07
 *  Author: Слава
 */ 

#ifndef PARAMS_H_
#define PARAMS_H_

#include <stdint.h>
#include <stdbool.h>
#include "ParamsDef.h"
#include "main.h"
#include "Output.h"
#include "Motors.h"
#include "GlobalConstants.h"
#include <avr/eeprom.h>


typedef enum
{
    eT      = 0,	//температура воздуха
    eP      = 1,	//атмосферное давление

    eZ      = 2,	//текущая высота
    epZ     = 3,	//предыдущая высота

    ea_x    = 4,	//Проекция ускорения на ось Ох
    ea_y    = 5,	//Проекция ускорения на ось Оу
    ea_z    = 6,	//Проекция ускорения на ось Oz

    ep_a_x  = 7,	//Проекция ускорения на ось Ох в предыдущем измерении
    ep_a_y  = 8,	//Проекция ускорения на ось Оу в предыдущем измерении
    ep_a_z  = 9,	//Проекция ускорения на ось Oz в предыдущем измерении

    ek_x    = 10,	//Проекция перегрузки на ось Ох
    ek_y    = 11,	//Проекция перегрузки на ось Оу
    ek_z    = 12,	//Проекция перегрузки на ось Oz

    ep_k_x  = 13,	//Проекция перегрузки на ось Ох в предыдущем измерении
    ep_k_y  = 14,	//Проекция перегрузки на ось Оу в предыдущем измерении
    ep_k_z  = 15,	//Проекция перегрузки на ось Oz в предыдущем измерении

    eV_x    = 16,	//Проекция скорости на ось Ох
    eV_y    = 17,	//Проекция скорости на ось Оу
    eV_z    = 18,	//Проекция скорости на ось Oz

    ew_x    = 19,	//Угловая скорость вокруг оси Ох
    ew_y    = 20,	//Угловая скорость вокруг оси Оу
    ew_z    = 21,	//Угловая скорость вокруг оси Oz

    ep_w_x  = 22,	//Угловая скорость вокруг оси Ох в предыдущем измерении
    ep_w_y  = 23,	//Угловая скорость вокруг оси Оу в предыдущем измерении
    ep_w_z  = 24,	//Угловая скорость вокруг оси Oz в предыдущем измерении

    eB_x    = 25,	//Проекция вектора магнитной индукции на ось Ox
    eB_y    = 26,	//Проекция вектора магнитной индукции на ось Oy
    eB_z    = 27,	//Проекция вектора магнитной индукции на ось Ox

    et_Z    = 28,	//заданная высота
    et_Vx   = 29,	//проекция заданной скорости на ось Ох
    et_Vy   = 30,	//проекция заданной скорости на ось Оу
    et_Oz    = 31,	//заданный курс
	eOz		= 32,	//текущий курс
	eTalt	= 33,	//резервный термометр
	ep_Oz	= 34,	//курс в предидущем измерении
	eOx		= 35,	//крен
	eOy		= 36,	//тангаж
	et_Ox	= 37,	//заданный крен
	et_Oy	= 38,	//заданный тангаж
	eint_w_x= 39,	//интегральная сумма - угол крена
	eint_w_y= 40,	//интегральная сумма - угол тангажа
	eacc_Ox = 41,	//акселерометр - угол крена
	eacc_Oy = 42,	//акселерометр - угол тангажа	
	eU_Batt	= 43,	//Voltage Bat
	ep_Ox	= 44,	//крен в предыдущем измерении
	ep_Oy	= 45,	//тангаж в предыдущем измерении
	eint_Ox	= 46,	//интегральная сумма - крена pId
	eint_Oy	= 47,	//интегральная сумма - тангажа pId
	eint_w_z= 48,	//интегральная сумма - угол рыскания
	emagOz	= 49,	//магнитный курс
	eint_Oz	= 50,	//интегральная сумма - отклонение от курса pId
	
    measurements_list_SIZE
} measurements_list;

typedef enum
{
	Kp_Oz	=	0,
	Ki_Oz	=	1,
	Kd_Oz	=	2,
	Kp_Ox	=	3,
	Ki_Ox	=	4,
	Kd_Ox	=	5,	
	Kp_Oy	=	6,
	Ki_Oy	=	7,
	Kd_Oy	=	8,
	ADXL345_OFFX	=	9,
	ADXL345_OFFY	=	10,
	ADXL345_OFFZ	=	11,	
	Kcomp_Ox		=	12,
	Kcomp_Oy		=	13,
	Kcomp_Oz		=	14,
	L3G4200D_OFFX	=	15,
	L3G4200D_OFFY	=	16,
	L3G4200D_OFFZ	=	17,
	AXL_TRUE_ACC	=	18,
	COR_X_LIM		=	19,
	COR_Y_LIM		=	20,
	COR_Z_LIM		=	21,
	LP_AX_X			=	22,
	LP_AX_Y			=	23,
	LP_AX_Z			=	24,
	HP_GYR_X		=	25,
	HP_GYR_Y		=	26,
	HP_GYR_Z		=	27,
	IntSum_LIM		=	28,
	derivX			=	29,
	derivY			=	30,
	derivZ			=	31,
	Mag_deviat		=	32,
	MAG_OffsetX		=	33,
	MAG_OffsetY		=	34,
	MAG_OffsetZ		=	35,
	My_ID			=	36,
	U_Alarm			=	37,
	INCL_TRUE_HEAD	=	38,
	Takeoff_Int_Lim =	39,
	ADXL345_SCX		=	40,
	ADXL345_SCY		=	41,
	ADXL345_SCZ		=	42,
	
	coeff_list_SIZE
		
} coeff_list;

typedef enum
{
	MB_MANUAL		=	0,
	MB_LED_RED		=	1,
	MB_LED_BLUE		=	2,
	MB_LED_GREEN	=	3,
	MB_SOUND		=	4,
	MB_FRONT_LEFT	=	5,
	MB_FRONT_RIGHT	=	6,
	MB_REAR_LEFT	=	7,
	MB_REAR_RIGHT	=	8,	
	MB_ALL			=	9,
	MB_Kp_Oz		=	10,
	MB_Ki_Oz		=	12,
	MB_Kd_Oz		=	14,
	MB_Kp_Ox		=	16,
	MB_Ki_Ox		=	18,
	MB_Kd_Ox		=	20,
	MB_Kp_Oy		=	22,
	MB_Ki_Oy		=	24,
	MB_Kd_Oy		=	26,
	MB_ADXL345_OFFX	=	28,
	MB_ADXL345_OFFY	=	30,
	MB_ADXL345_OFFZ	=	32,
	MB_Kcomp_Ox		=	34,
	MB_Kcomp_Oy		=	36,
	MB_Kcomp_Oz		=	38,
	MB_L3G4200D_OFFX=	40,
	MB_L3G4200D_OFFY=	42,
	MB_L3G4200D_OFFZ=	44,
	MB_AXL_TRUE_ACC	=	46,
	MB_COR_X_LIM	=	48,
	MB_COR_Y_LIM	=	50,
	MB_COR_Z_LIM	=	52,
	MB_LP_AX_X		=	54,
	MB_LP_AX_Y		=	56,
	MB_LP_AX_Z		=	58,
	MB_HP_GYR_X		=	60,
	MB_HP_GYR_Y		=	62,
	MB_HP_GYR_Z		=	64,
	MB_IntSum_LIM	=	66,
	MB_derivX		=	68,
	MB_derivY		=	70,
	MB_derivZ		=	72,
	MB_Mag_deviat	=	74,
	MB_OFFSET		=	10
}	modbus_params;

//адреса EEPROM
typedef enum
{
	EE_Kp_Oz			=	0,
	EE_Ki_Oz			=	4,
	EE_Kd_Oz			=	8,
	EE_Kp_Ox			=	12,
	EE_Ki_Ox			=	16,
	EE_Kd_Ox			=	20,
	EE_Kp_Oy			=	24,
	EE_Ki_Oy			=	28,
	EE_Kd_Oy			=	32,
	EE_ADXL345_OFFX		=	36,
	EE_ADXL345_OFFY		=	40,
	EE_ADXL345_OFFZ		=	44,
	EE_Kcomp_Ox			=	48,
	EE_Kcomp_Oy			=	52,
	EE_Kcomp_Oz			=	56,
	EE_L3G4200D_OFFX	=	60,
	EE_L3G4200D_OFFY	=	64,
	EE_L3G4200D_OFFZ	=	68,
	EE_AXL_TRUE_ACC		=	72,
	EE_COR_X_LIM		=	76,
	EE_COR_Y_LIM		=	80,
	EE_COR_Z_LIM		=	84,
	EE_LP_AX_X			=	88,
	EE_LP_AX_Y			=	92,
	EE_LP_AX_Z			=	96,
	EE_HP_GYR_X			=	100,
	EE_HP_GYR_Y			=	104,
	EE_HP_GYR_Z			=	108,
	EE_IntSum_LIM		=	112,
	EE_derivX			=	116,
	EE_derivY			=	120,
	EE_derivZ			=	124,
	EE_Mag_deviat		=	128
		
}	eeprom_params;

extern parametr_t telemetry[measurements_list_SIZE];
extern parametr_t coeffs[coeff_list_SIZE];

#define T       telemetry[eT]	//температура воздуха
#define P       telemetry[eP]	//атмосферное давление

#define H       telemetry[eZ]	//текущая высота
#define pZ      telemetry[epZ]	//предыдущая высота

#define a_x     telemetry[ea_x]	//Проекция ускорения на ось Ох
#define a_y     telemetry[ea_y]	//Проекция ускорения на ось Оу
#define a_z     telemetry[ea_z]	//Проекция ускорения на ось Oz

#define p_a_x   telemetry[ep_a_x]	//Проекция ускорения на ось Ох в предыдущем измерении
#define p_a_y   telemetry[ep_a_y]	//Проекция ускорения на ось Оу в предыдущем измерении
#define p_a_z   telemetry[ep_a_z]	//Проекция ускорения на ось Oz в предыдущем измерении

#define k_x     telemetry[ek_x]	//Проекция перегрузки на ось Ох
#define k_y     telemetry[ek_y]	//Проекция перегрузки на ось Оу
#define k_z     telemetry[ek_z]	//Проекция перегрузки на ось Oz

#define p_k_x   telemetry[ep_k_x]	//Проекция перегрузки на ось Ох в предыдущем измерении
#define p_k_y   telemetry[ep_k_y]	//Проекция перегрузки на ось Оу в предыдущем измерении
#define p_k_z   telemetry[ep_k_z]	//Проекция перегрузки на ось Oz в предыдущем измерении

#define V_x     telemetry[eV_x]	//Проекция скорости на ось Ох
#define V_y     telemetry[eV_y]	//Проекция скорости на ось Оу
#define V_z     telemetry[eV_z]	//Проекция скорости на ось Oz

#define w_x     telemetry[ew_x]	//Угловая скорость вокруг оси Ох
#define w_y     telemetry[ew_y]	//Угловая скорость вокруг оси Оу
#define w_z     telemetry[ew_z]	//Угловая скорость вокруг оси Oz

#define p_w_x   telemetry[ep_w_x]	//Угловая скорость вокруг оси Ох в предыдущем измерении
#define p_w_y   telemetry[ep_w_y]	//Угловая скорость вокруг оси Оу в предыдущем измерении
#define p_w_z   telemetry[ep_w_z]	//Угловая скорость вокруг оси Oz в предыдущем измерении

#define B_x     telemetry[eB_x]	//Проекция вектора магнитной индукции на ось Ox
#define B_y     telemetry[eB_y]	//Проекция вектора магнитной индукции на ось Oy
#define B_z     telemetry[eB_z]	//Проекция вектора магнитной индукции на ось Ox

#define t_Z     telemetry[et_Z]		//заданная высота
#define t_Vx    telemetry[et_Vx]	//проекция заданной скорости на ось Ох
#define t_Vy    telemetry[et_Vy]	//проекция заданной скорости на ось Оу
#define t_Oz     telemetry[et_Oz]		//заданный курс
#define p_Oz	telemetry[ep_Oz]		//курс в предидущем измерении
#define Oz		telemetry[eOz]		//текущий курс
#define Talt	telemetry[eTalt]	//резервный термометр
#define Ox		telemetry[eOx]		//крен
#define Oy		telemetry[eOy]		//тангаж
#define t_Ox	telemetry[et_Ox]	//заданный крен
#define t_Oy	telemetry[et_Oy]	//заданный тангаж
#define int_w_x	telemetry[eint_w_x]	//интегральная сумма - угол крена	
#define int_w_y	telemetry[eint_w_y]	//интегральная сумма - угол тангажа
#define acc_Ox	telemetry[eacc_Ox]	//акселерометр - угол крена
#define acc_Oy	telemetry[eacc_Oy]	//акселерометр - угол тангажа
#define U_Batt	telemetry[eU_Batt]	//Voltage
#define p_Ox	telemetry[ep_Ox]	//крен в предыдущем измерении
#define	p_Oy	telemetry[ep_Oy]	//тангаж в предыдущем измерении
#define int_Ox	telemetry[eint_Ox]	//интегральная сумма - крена pId
#define int_Oy	telemetry[eint_Oy]	//интегральная сумма - тангажа pId
#define int_w_z	telemetry[eint_w_z]	//интегральная сумма - угол рыскания
#define magOz	telemetry[emagOz]	//магнитный курс
#define int_Oz  telemetry[eint_Oz]	//интегральная сумма - отклонение от курса pId

void ModbusLoader();
void ModbusSaver();
void ModbusInitValues();

#endif /* PARAMS_H_ */
