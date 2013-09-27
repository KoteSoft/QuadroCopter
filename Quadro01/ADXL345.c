/*
 * ADXL345.c
 *
 * Created: 27.06.2013 13:47:38
 *  Author: Слава
 */ 
#include "ADXL345.h"



uint8_t ADXL345_Poll()
{
	uint8_t LSB, MSB;
	
	twi(TWI_START);
	TWDR=ADXL345_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x32;
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR=ADXL345_ADDR|I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	LSB=TWDR;
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	p_k_y.value = k_y.value;
	k_y.value = (-0.0032) * (double)(MSB << 8 | LSB);
	//k_y.value = LowPassFilter((-0.0032) * (double)(MSB << 8 | LSB), p_k_y.value - coeffs[ADXL345_OFFY].value, coeffs[LP_AX_Y].value);
	k_y.value += coeffs[ADXL345_OFFY].value;
	k_y.value *= coeffs[ADXL345_SCY].value;
	//Measure_Push((-0.0032) * (double)(MSB << 8 | LSB) + coeffs[ADXL345_OFFY].value, KY);
	//Measure_Push((MSB << 8 | LSB), KY);
	
	
	twi(TWI_RECEIVE_ACK);
	LSB=TWDR;
	twi(TWI_RECEIVE_ACK);
	MSB=TWDR;
	p_k_x.value = k_x.value;
	k_x.value = (-0.0032) * (double)(MSB << 8 | LSB);
	//k_x.value = LowPassFilter((-0.0032) * (double)(MSB << 8 | LSB), p_k_x.value - coeffs[ADXL345_OFFX].value, coeffs[LP_AX_X].value);
	k_x.value += coeffs[ADXL345_OFFX].value;
	k_x.value *= coeffs[ADXL345_SCX].value;
	//Measure_Push((MSB << 8 | LSB), KX);
	
	twi(TWI_RECEIVE_ACK);
	LSB=TWDR;
	twi(TWI_RECEIVE_NACK);
	MSB=TWDR;
	twi(TWI_STOP);
	p_k_z.value = k_z.value;
	k_z.value = (0.0032) * (double)(MSB << 8 | LSB);
	//k_z.value = LowPassFilter((0.0032) * (double)(MSB << 8 | LSB), p_k_z.value - coeffs[ADXL345_OFFZ].value, coeffs[LP_AX_Z].value);
	k_z.value += coeffs[ADXL345_OFFZ].value;
	k_z.value *= coeffs[ADXL345_SCZ].value;
	//Measure_Push((MSB << 8 | LSB), KZ);
	
	return 0;
}

uint8_t ADXL345_Init()
{
	twi(TWI_START);
	TWDR=ADXL345_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x31;	//DATA_FORMAT
	twi(TWI_TRANSMIT);
	TWDR=0b00001011;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR=ADXL345_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x2C;	//BW_RATE
	twi(TWI_TRANSMIT);
	TWDR=0b00000111;	//0x09; //0b00001111 rate=50hz, bw=20hz 
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR=ADXL345_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x2D;	//POWER_CTL 
	twi(TWI_TRANSMIT);
	TWDR=0x08;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	/*
	twi(TWI_START);
	TWDR=ADXL345_ADDR|I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR=0x2E;	//INT_ENABLE
	twi(TWI_TRANSMIT);
	TWDR=0x80;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	*/
	return 0;
}

