/*
 * L3G4200D.c
 *
 * Created: 29.06.2013 15:29:54
 *  Author: Слава
 */ 
#include "L3G4200D.h"

uint8_t L3G4200D_Init()
{
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x20; //CTRL_REG1
	twi(TWI_TRANSMIT);
	TWDR = 0<<DR1 | 1<<DR0 | 0<<BW1 | 0<<BW0 | 1<<PD | 1<<Zen | 1<<Yen | 1<<Xen;	
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x21; //CTRL_REG2
	twi(TWI_TRANSMIT);
	TWDR = 0;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x22; //CTRL_REG3
	twi(TWI_TRANSMIT);
	TWDR = 0;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x23; //CTRL_REG4
	twi(TWI_TRANSMIT);
	TWDR = 0<<BDU | 0<<BLE | 1<<FS1 | 1<<FS0 | 0<<ST1 | 0<<ST0 | 0<<SIM;	//TWDR = 1<<BDU | 0<<BLE | 0<<FS1 | 0<<FS0 | 0<<ST1 | 0<<ST0 | 0<<SIM;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x24; //CTRL_REG5
	twi(TWI_TRANSMIT);
	TWDR = 0<<BOOT | 0<<FIFO_EN | 0<<HPen | 0<<INT1_Sel1 | 0<<INT1_Sel0 | 1<<Out_Sel1 | 0<<Out_Sel0;	//TWDR = 0<<BOOT | 0<<FIFO_EN | 0<<HPen | 0<<INT1_Sel1 | 0<<INT1_Sel0 | 0<<Out_Sel1 | 0<<Out_Sel0;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);		
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x2E; //FIFO_CTRL_REG
	twi(TWI_TRANSMIT);
	TWDR = 0<<FM2 | 0<<FM1 | 0<<FM0 | 0<<WTM4 | 0<<WTM3 | 0<<WTM2 | 0<<WTM1 | 0<<WTM0;
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	return 0;
	
}

uint8_t L3G4200D_Poll()
{
	uint8_t MSB, LSB;
	
	
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x26; //OUT_TEMP
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR = L3G4200D_ADDR | I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_NACK);
	LSB = TWDR;
	twi(TWI_STOP);	
	
	Talt.value = (int8_t)LSB;
	
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x28 | 0b10000000; //OUT_X_L
	twi(TWI_TRANSMIT);
	twi(TWI_RESTART);
	TWDR = L3G4200D_ADDR | I2C_READ;
	twi(TWI_TRANSMIT);
	twi(TWI_RECEIVE_ACK);
	LSB = TWDR;
	twi(TWI_RECEIVE_ACK);
	MSB = TWDR;	
	p_w_y.value = w_y.value;
	if(fabs((MSB << 8 | LSB) * 0.00875 - p_w_y.value) > coeffs[L3G4200D_OFFY].value)
	{
		w_y.value = (MSB << 8 | LSB) * 0.00875;
	}
	else
	{
		w_y.value = p_w_y.value;
	}
	
	//Measure_Push((MSB << 8 | LSB) * 0.00875, WY);
	//Measure_Push((MSB << 8 | LSB), WY);
	
	twi(TWI_RECEIVE_ACK);
	LSB = TWDR;
	twi(TWI_RECEIVE_ACK);
	MSB = TWDR;
	
	p_w_x.value = w_x.value;
	if(fabs((MSB << 8 | LSB) * 0.00875 - p_w_x.value) > coeffs[L3G4200D_OFFX].value)
	{
		w_x.value = (MSB << 8 | LSB) * 0.00875;
		//wx_sum[peak] = (((MSB << 8 | LSB) >> SHIFT_FILTER) << SHIFT_FILTER) * 0.00875;
	}
	else
	{
		w_x.value = p_w_x.value;
		//wx_sum[peak] = 0.0;
	}
	
	//Measure_Push((MSB << 8 | LSB) * 0.00875, WX);
	//Measure_Push((MSB << 8 | LSB), WX);
	
	twi(TWI_RECEIVE_ACK);
	LSB = TWDR;
	twi(TWI_RECEIVE_NACK);
	MSB = TWDR;
	
	p_w_z.value = w_z.value;
	if(fabs((MSB << 8 | LSB) * 0.00875 - p_w_z.value) > coeffs[L3G4200D_OFFZ].value)
	{
		w_z.value = (MSB << 8 | LSB) * 0.00875;
		//wz_sum[peak] = (((MSB << 8 | LSB) >> SHIFT_FILTER) << SHIFT_FILTER) * 0.00875;
	}
	else
	{
		w_z.value = p_w_z.value;
		//wz_sum[peak] = 0.0;
	}
	
	//Measure_Push((MSB << 8 | LSB) * 0.00875, WZ);
	//Measure_Push((MSB << 8 | LSB), WZ);
	
	twi(TWI_STOP);
	return 0;
}

uint8_t L3G4200D_Test(uint8_t mode)
{
	twi(TWI_START);
	TWDR = L3G4200D_ADDR | I2C_WRITE;
	twi(TWI_TRANSMIT);
	TWDR = 0x23; //CTRL_REG4
	twi(TWI_TRANSMIT);
	
	switch (mode)
	{
		case 0:
		{
			TWDR = 1<<BDU | 0<<BLE | 0<<FS1 | 0<<FS0 | 0<<ST1 | 0<<ST0 | 0<<SIM;
			break;
		}
		case 1:
		{
			TWDR = 1<<BDU | 0<<BLE | 0<<FS1 | 0<<FS0 | 0<<ST1 | 1<<ST0 | 0<<SIM;
			break;
		}
		case 2:
		{
			TWDR = 1<<BDU | 0<<BLE | 0<<FS1 | 0<<FS0 | 1<<ST1 | 1<<ST0 | 0<<SIM;
			break;
		}
		default:
		{
			TWDR = 1<<BDU | 0<<BLE | 0<<FS1 | 0<<FS0 | 0<<ST1 | 0<<ST0 | 0<<SIM;
		}
	}	
	
	twi(TWI_TRANSMIT);
	twi(TWI_STOP);
	
	return 0;
}