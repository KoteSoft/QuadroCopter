/*
 * SensorsPoll.c
 *
 * Created: 25.06.2013 21:19:59
 *  Author: Слава
 */ 

#include "SensorsPoll.h"

uint8_t SensorsInit()
{
	BMP085_Get_Coef();
	ADXL345_Init();
	HMC5883L_Init();
	L3G4200D_Init();
	
	peak = 0;
	
	int_w_x.value = 0.0;
	int_w_y.value = 0.0;
	int_Ox.value = 0.0;
	int_Oy.value = 0.0;
	
	return 0;
}

static uint8_t BMP085PreTimer = 0;
static uint8_t BMP085Flag = 0;

uint8_t SensorsPoll()
{
	BMP085PreTimer++;
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 0) && BMP085PreTimer >= 4 && BMP085Flag)
	{
		BMP085_GetUT();
		BMP085_StartMeasUP();
		BMP085PreTimer = 0;
		BMP085Flag = 0;
	}		
	else if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 0) && BMP085PreTimer >= 4 && !BMP085Flag)
	{
		BMP085_GetUP();
		BMP085_StartMeasUT();
		BMP085PreTimer = 0;
		BMP085Flag = 1;
	}	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 1))
	{
		ADXL345_Poll();
	}	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 2))
	{
		HMC5883L_Poll();
	}	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 3))
	{
		L3G4200D_Poll();
	}		
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 5))
	{
		L3G4200D_Test(1);
	}
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 6))
	{
		L3G4200D_Test(2);
	}
	
	if (bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 5) && bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 6))
	{
		L3G4200D_Test(0);
	}
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 7) && bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 8))
	{
		HMC5883L_Test(1);
	}
	
	if (bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 7) && bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 8))
	{
		HMC5883L_Test(2);
	}
	
	if (bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 7) && bit_is_clear(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 8))
	{
		HMC5883L_Test(0);
	}
	
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
	
	if (U_Batt.value < coeffs[U_Alarm].value)
	{
		usRegHoldingBuf[MB_OFFSET+MB_SOUND] |= 2;
	}
	else
	{
		usRegHoldingBuf[MB_OFFSET+MB_SOUND] &= ~2;
	}
		
	
	return 0;
}

void Angular_Calc()
{
	acc_Ox.value = -1.0 * (atan2( k_y.value, k_z.value) * 180.0) / M_PI;
	acc_Oy.value = (atan2( k_x.value, k_z.value) * 180.0) / M_PI;
	
	//интеграл за последний период измерения
	int_w_x.value += ((w_x.value + p_w_x.value) / 2.0) * Step;
	int_w_y.value += ((w_y.value + p_w_y.value) / 2.0) * Step;
	int_w_z.value += ((w_z.value + p_w_z.value) / 2.0) * Step;
	//Oz_gyr = ((w_z + p_w_z) / 2.0) * Step;
	
	//Коррекция ошибки интегрирования угловой скорости
	if ((fabs(fabs(k_x.value) + fabs(k_y.value) + fabs(k_z.value) - 1.0) < coeffs[AXL_TRUE_ACC].value) && fabs(int_w_x.value - acc_Ox.value) < coeffs[COR_X_LIM].value && fabs(int_w_y.value - acc_Oy.value) < coeffs[COR_Y_LIM].value)
	{
		int_w_x.value = acc_Ox.value;
		int_w_y.value = acc_Oy.value;
	}
	
	p_Ox.value = Ox.value;
	p_Oy.value = Oy.value;
	p_Oz.value = Oz.value;
	
	//Комплементарный фильтр
	Ox.value = ComplemetaryFilter(acc_Ox.value, int_w_x.value, coeffs[Kcomp_Ox].value);
	Oy.value = ComplemetaryFilter(acc_Oy.value, int_w_y.value, coeffs[Kcomp_Oy].value);
	Oz.value = ComplemetaryFilter(magOz.value, int_w_z.value, coeffs[Kcomp_Oz].value);
	
	//Накапливаем интегральные суммы для И-крена и И-тангажа для ПИДов
	if (p_Ox.value > -90.0 && p_Ox.value < 90.0 && p_Oy.value > -90.0 && p_Oy.value < 90.0)	//проверка значения углов на корректность (включен ли инклинометр?)
	{
		int_Ox.value += ((Ox.value + p_Ox.value) / 2.0) * Step;
		int_Oy.value += ((Oy.value + p_Oy.value) / 2.0) * Step;
	}
	else 
	{
		int_Ox.value = 0.0;
		int_Oy.value = 0.0;
	}
	
	//Ограничение интегральных сумм ПИДов И-крена и И-тангажа
	if (int_Ox.value > coeffs[IntSum_LIM].value)
	{
		int_Ox.value = coeffs[IntSum_LIM].value;
	}
	if (int_Oy.value > coeffs[IntSum_LIM].value)
	{
		int_Oy.value = coeffs[IntSum_LIM].value;
	}
	if (int_Ox.value < coeffs[IntSum_LIM].value * (-1.0))
	{
		int_Ox.value = coeffs[IntSum_LIM].value * (-1.0);
	}
	if (int_Oy.value < coeffs[IntSum_LIM].value * (-1.0))
	{
		int_Oy.value = coeffs[IntSum_LIM].value * (-1.0);
	}
	
	//отключаем накопление ошибок на взлете
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 9))
	{
		int_Ox.value = coeffs[Takeoff_Int_Lim].value;
		int_Oy.value = coeffs[Takeoff_Int_Lim].value;
	}
}

double_t Heading_displacement_calc(double_t Current, double_t Target)
{
	double_t X = Target - Current;
	if (fabs(X) < 180.0)
	return X;

	if (X > 0.0)
	{
		X -= 360.0;
	}
	else
	{
		X += 360.0;
	}

	return X;
}
