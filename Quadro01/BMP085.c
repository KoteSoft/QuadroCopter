/*
 * BMP085.c
 *
 * Created: 26.06.2013 22:52:08
 *  Author: Слава
 */ 
#include "BMP085.h"

static int16_t AC1;
static int16_t AC2;
static int16_t AC3;
static uint16_t AC4;
static uint16_t AC5;
static uint16_t AC6;
static int16_t B1;
static int16_t B2;
static int16_t MB;
static int16_t MC;
static int16_t MD;

static int32_t UT;
static int32_t UP;

//const uint8_t oss = 3;

void BMP085_Calculate();

uint8_t BMP085_StartMeasUT()
{
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xF4;
	twi(TWI_TRANSMIT);
	TWDR=0x2E;	//temperature
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	return 0; 
}

uint8_t BMP085_StartMeasUP()
{
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x34;
	twi(TWI_TRANSMIT);
	TWDR = 0x34 + oss * 0x40;	//pressure	
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	return 0;
}

uint8_t BMP085_GetUT()
{
	uint8_t LSB, MSB, XLSB;	
	//_delay_ms(20);
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xF6;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	UT = (int32_t)(MSB<<8 | LSB);	
	BMP085_Calculate();
	return 0;
}

uint8_t BMP085_GetUP()
{
	volatile int32_t LSB, MSB, XLSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xF6;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_ACK);
	LSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	XLSB=TWDR;
	twi(TWI_STOP);
	UP = (MSB<<16 | LSB<<8 | XLSB) >> (8 - oss);
	BMP085_Calculate();
	return 0;
}

uint8_t BMP085_Get_Coef()
{
	uint16_t LSB, MSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xAA;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC1 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xAC;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC2 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xAE;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC3 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xB0;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC4 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xB2;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC5 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xB4;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	AC6 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xB6;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	B1 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xB8;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	B2 = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xBA;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	MB = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xBC;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	MC = MSB<<8 | LSB;
	
	twi(TWI_START);
	TWDR=BMP085_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0xBE;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=BMP085_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	LSB=TWDR;
	twi(TWI_STOP);
	
	MD = MSB<<8 | LSB;
		
	return 0;
}

void BMP085_Calculate()
{
	volatile int32_t X1, X2, X3, B3, B5, B6, p, tmp;
	volatile uint32_t B4, B7;
	
	X1 = ((int32_t)UT - AC6) * AC5 >> 15;
	X2 = ((int32_t)MC << 11) / (X1 + MD);
	B5 = X1 + X2;
	
	T.value = (double_t)(0.1 * ((B5 + 8) >> 4));
	
	B6 = B5 - 4000;
	X1 = (B2 * (B6 * B6 >> 12)) >> 11;
	X2 = AC2 * B6 >> 11;
	X3 = X1 + X2;
	tmp = AC1;
	tmp = (tmp * 4 + X3) << oss;
	B3 = (tmp + 2) / 4;
	X1 = AC3 * B6 >> 13;
	X2 = (B1 * (B6 * B6 >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;
	//B7 = ((uint32_t)(UP >> (8 - oss)) - B3) * (50000 >> oss);
	B7 = ((uint32_t) (UP - B3) * (50000 >> oss));
	p = B7 < 0x80000000 ? (B7 * 2) / B4 : (B7 / B4) * 2;	
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
				
	P.value = Pa2mmHg * (double_t)(p + ((X1 + X2 + 3791) >> 4));
}

/*
void BMP085_Calculate()
{
	int32_t X1, X2, X3, B3, B5, B6, p;
	uint32_t B4, B7;
	
	X1 = ((int32_t)UT - (int32_t)AC6 ) * (int32_t)AC5 >> 15;
	X2 = ((int32_t)MC << 11) / (X1 + MD);
	B5 = X1 + X2;
	
	T.value = (double_t)(0.1 * ((B5 + 8) >> 4));
	//T.value = (double_t)12.345;
	
	B6 = B5 - 4000;
	X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
	X2 = (AC2 * B6) >> 11;
	X3 = X1 + X2;
	B3 = (((((int32_t) AC1) * 4 + X3) << oss) + 2) >> 2;
	X1 = (AC3 * B6) >> 13;
	X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (uint32_t) (X3 + 32768)) >> 15;
	B7 = ((uint32_t) (UP - B3) * (50000 >> oss));
	if (B7 < 0x80000000)
	{
		p = (B7 << 1) / B4;
	}
	else
	{
		p = (B7 / B4) << 1;
	}
	
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	
	P.value = (p + ((X1 + X2 + 3791) >> 4));
}
*/
