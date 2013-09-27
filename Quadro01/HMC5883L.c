/*
 * HMC5883L.c
 *
 * Created: 27.06.2013 18:14:33
 *  Author: Слава
 */ 
#include "HMC5883L.h"

uint8_t HMC5883L_Init()
{
	twi(TWI_START);
	TWDR = HMC5883L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x00;	//CRA
	twi(TWI_TRANSMIT);
	TWDR = 0b01110000;	//8-average, 15 Hz default, normal measurement
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = HMC5883L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x01;	//CRB
	twi(TWI_TRANSMIT);
	TWDR = 0b00100000;	//Gain=5, or any other desired gain
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = HMC5883L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x02;	//Mode
	twi(TWI_TRANSMIT);
	TWDR = 0b10000000;	//Continuous-measurement mode
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	return 0;
}

uint8_t HMC5883L_Poll()
{
	uint8_t MSBX, MSBY, MSBZ, LSBX, LSBY, LSBZ;

	twi(TWI_START);
	TWDR = HMC5883L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x03;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR = HMC5883L_ADDR | I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);	
	MSBX = TWDR;
	twi(TWI_RECEIVE_ACK);
	LSBX = TWDR;
	twi(TWI_RECEIVE_ACK);
	MSBZ = TWDR;
	twi(TWI_RECEIVE_ACK);	
	LSBZ = TWDR;
	twi(TWI_RECEIVE_ACK);
	MSBY =TWDR;
	twi(TWI_RECEIVE_NACK);
	LSBY = TWDR;
	twi(TWI_STOP);
	
	B_x.value = (double)(MSBX << 8 | LSBX) / 1370.0 + coeffs[MAG_OffsetX].value;
	B_y.value = (double)(MSBY << 8 | LSBY) / 1370.0 + coeffs[MAG_OffsetY].value;
	B_z.value = (double)(MSBZ << 8 | LSBZ) / 1370.0	+ coeffs[MAG_OffsetZ].value;
	
	return 0;
}

uint8_t Course_Calc()
{
	double_t A=0.0;
	
	volatile double_t Bx, By, Bz, Q, B_x_read, B_y_read, B_z_read;
	
	typedef struct
	{
		double_t X;
		double_t Y;
		double_t Z;	
	}vector;
	
	//Угол отклонения от вертикали в радианах
	Q = (-1.0) * acos((k_z.value)/(sqrt(k_x.value * k_x.value + k_y.value * k_y.value + k_z.value * k_z.value)));
	//int_Oz.value = Q * 180.0 / M_PI; //УБРАТЬ!!!!!!!!!!!!	
	//Нормаль к вектору (kx, ky, 0) - ось вращения
	volatile vector n;
	
	n.X = k_y.value;
	n.Y = (-1.0) * k_x.value;
	n.Z = 0.0;	
	
	//Нормализуем вектор нормали
	double_t n_L = sqrt(n.X*n.X+n.Y*n.Y);
	n.X /= n_L;
	n.Y /= n_L;
	
	B_x_read = B_x.value;
	B_y_read = B_y.value;
	B_z_read = B_z.value;       

	//Поворот вектора В. Компенсация отклонения магнетометра от вертикали
	Bx = B_x_read * (cos(Q) + (1.0 - cos(Q)) * (n.X * n.X)) + B_y_read * ((1.0 - cos(Q)) * n.X * n.Y - (sin(Q)) * n.Z) + B_z_read * ((1.0 - cos(Q)) * n.X * n.Z + sin(Q) * n.Y);
	By = B_x_read * ((1.0 - cos(Q)) * n.Y * n.X + sin(Q) * n.Z) + B_y_read * (cos(Q) + (1 - cos(Q)) * n.Y * n.Y) + B_z_read * ((1 - cos(Q)) * n.Y * n.Z - sin(Q) * n.X);
	//Bz = B_x.value * ((1.0 - cos(Q)) * n.Z * n.X - sin(Q) * n.Y) + B_y.value * ((1 - cos(Q)) * n.Z * n.Y + sin(Q) * n.X) + B_z.value * (cos(Q) + (1.0 - cos(Q)) * (n.Z * n.Z));
	
	//Расчет магнитного курса в радианах
	A = atan2(By, Bx);
	//A = atan2(B_y.value, B_x.value);
	
	//Коррекция на 0-2Пи
	if (A < 0.0)
	{
		A += 2 * M_PI;
	}
	else if (A > 2 * M_PI)
	{
		A -= 2 * M_PI;
	}
	
	//Перевод в градусы + повернутый магнитометр + магнитная девиация
	//magOz.value = A * 180.0 / M_PI;
	A = A * 180.0 / M_PI - 90.0 + coeffs[Mag_deviat].value;
	
	if (A >= 360.0)
	{
		A -= 360.0;
	}
	else if (A < 0)
	{
		A = 360.0 + A;
	}
	
	magOz.value = A;
	
	if (Q * 180.0 / M_PI < coeffs[INCL_TRUE_HEAD].value)
	{
		int_w_z.value = magOz.value;
	}
	
	return 0;
}

uint8_t HMC5883L_Test(uint8_t mode)
{
	twi(TWI_START);
	TWDR = HMC5883L_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x00;	//CRA
	twi(TWI_TRANSMIT);
	
	switch (mode)
	{
		case 0:
		{
			TWDR = 0b01111000;	//8-average, 15 Hz default, normal measurement
			break;
		}
		case 1:
		{
			TWDR = 0b01111001;	//8-average, 15 Hz default, normal measurement. Positive bias configuration for X, Y, and Z axes. 
			break;
		}
		case 2:
		{
			TWDR = 0b01111010;	//8-average, 15 Hz default, normal measurement. Negative bias configuration for X, Y and Z axes.
			break;
		}
		default:
		{
			TWDR = 0b01111000;	//8-average, 15 Hz default, normal measurement
		}
	}
	
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	return 0;	
}