/*
 * Params.c
 *
 * Created: 12.06.2013 17:12:24
 *  Author: Слава
 */ 

#include "Params.h"

parametr_t telemetry[measurements_list_SIZE]; 
parametr_t coeffs[coeff_list_SIZE];

void ModbusEEPROMLoader();

//Выводим телеметрию и т.п. в Inputs
void ModbusLoader()
{
	
	usRegInputBuf[1]=T.array[0];
	usRegInputBuf[2]=T.array[1];
	
	usRegInputBuf[3]=P.array[0];
	usRegInputBuf[4]=P.array[1];
	//usRegInputBuf[3]=(uint16_t)EEE;
	
	usRegInputBuf[5]=k_x.array[0];
	usRegInputBuf[6]=k_x.array[1];
	
	usRegInputBuf[7]=k_y.array[0];
	usRegInputBuf[8]=k_y.array[1];
	
	usRegInputBuf[9]=k_z.array[0];
	usRegInputBuf[10]=k_z.array[1];
	
	usRegInputBuf[11] = B_x.array[0];
	usRegInputBuf[12] = B_x.array[1];
	
	usRegInputBuf[13] = B_y.array[0];
	usRegInputBuf[14] = B_y.array[1];
	
	usRegInputBuf[15] = B_z.array[0];
	usRegInputBuf[16] = B_z.array[1];
	
	usRegInputBuf[17] = magOz.array[0];
	usRegInputBuf[18] = magOz.array[1];
	
	usRegInputBuf[19] = Talt.array[0];	
	usRegInputBuf[20] = Talt.array[1];
	
	usRegInputBuf[21] = w_x.array[0];
	usRegInputBuf[22] = w_x.array[1];
	
	usRegInputBuf[23] = w_y.array[0];
	usRegInputBuf[24] = w_y.array[1];
	
	usRegInputBuf[25] = w_z.array[0];
	usRegInputBuf[26] = w_z.array[1];
	
	usRegInputBuf[27] = Ox.array[0];
	usRegInputBuf[28] = Ox.array[1];
	
	usRegInputBuf[29] = Oy.array[0];
	usRegInputBuf[30] = Oy.array[1];
	
	usRegInputBuf[31] = int_w_x.array[0];
	usRegInputBuf[32] = int_w_x.array[1];
	
	usRegInputBuf[33] = int_w_y.array[0];
	usRegInputBuf[34] = int_w_y.array[1];
	
	usRegInputBuf[35] = acc_Ox.array[0];
	usRegInputBuf[36] = acc_Ox.array[1];
	
	usRegInputBuf[37] = acc_Oy.array[0];
	usRegInputBuf[38] = acc_Oy.array[1];
	
	usRegInputBuf[39] = U_Batt.array[0];
	usRegInputBuf[40] = U_Batt.array[1];
	
	usRegInputBuf[41] = int_Ox.array[0];
	usRegInputBuf[42] = int_Ox.array[1];
	
	usRegInputBuf[43] = int_Oy.array[0];
	usRegInputBuf[44] = int_Oy.array[1];
	
	usRegInputBuf[45] = int_w_z.array[0];
	usRegInputBuf[46] = int_w_z.array[1];
	
	usRegInputBuf[47] = int_Oz.array[0];
	usRegInputBuf[48] = int_Oz.array[1];
	
	usRegInputBuf[49] = Oz.array[0];
	usRegInputBuf[50] = Oz.array[1];	
}

//Обрабатываем значения HoldingRegisters
void ModbusSaver()
{
	if (usRegHoldingBuf[MB_OFFSET+MB_LED_RED])
	{
		LED_On(LED_RED);
	} 
	else
	{
		LED_Off(LED_RED);
	}
	
	if (usRegHoldingBuf[MB_OFFSET+MB_LED_BLUE])
	{
		LED_On(LED_BLUE);
	}
	else
	{
		LED_Off(LED_BLUE);
	}
	
	if (usRegHoldingBuf[MB_OFFSET+MB_LED_GREEN])
	{
		LED_On(LED_GREEN);
	}
	else
	{
		LED_Off(LED_GREEN);
	}
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_SOUND], 0))
	{
		Sound_On();
	}
	else
	{
		Sound_Off();
	}
	
	if (usRegHoldingBuf[MB_OFFSET+MB_ALL]<16000UL)
	{
		usRegHoldingBuf[MB_OFFSET+MB_ALL]=16000UL;
	}
	
	
	if (bit_is_set(usRegHoldingBuf[MB_OFFSET+MB_MANUAL], 4))
	{
		double_t speeds[4];
		speeds[FRONT_LEFT]	= (double_t)usRegHoldingBuf[MB_OFFSET + MB_FRONT_LEFT];
		speeds[FRONT_RIGHT] = (double_t)usRegHoldingBuf[MB_OFFSET + MB_FRONT_RIGHT];
		speeds[REAR_LEFT]	= (double_t)usRegHoldingBuf[MB_OFFSET + MB_REAR_LEFT];
		speeds[REAR_RIGHT]	= (double_t)usRegHoldingBuf[MB_OFFSET + MB_REAR_RIGHT];
		SetMotors(speeds);	
	}
	else
	{
		usRegHoldingBuf[MB_OFFSET + MB_FRONT_LEFT] = counter[FRONT_LEFT];
		usRegHoldingBuf[MB_OFFSET + MB_FRONT_RIGHT] = counter[FRONT_RIGHT];
		usRegHoldingBuf[MB_OFFSET + MB_REAR_LEFT] = counter[REAR_LEFT];
		usRegHoldingBuf[MB_OFFSET + MB_REAR_RIGHT] = counter[REAR_RIGHT];
	}		
	
	//t_Ox.value = 0;
	//t_Oy.value = 0;
	t_Ox.array[0] = usRegHoldingBuf[2];
	t_Ox.array[1] = usRegHoldingBuf[3];
		
	t_Oy.array[0] = usRegHoldingBuf[4];
	t_Oy.array[1] = usRegHoldingBuf[5];
	
	t_Oz.array[0] = usRegHoldingBuf[6];
	t_Oz.array[1] = usRegHoldingBuf[7];
		
	ModbusEEPROMLoader();
}

//Загружаем в Holding Registers и в массив параметров значения из EEPROM
void ModbusInitValues()
{	
	for(uint8_t i = 0; i < coeff_list_SIZE; i++)
	{
		coeffs[i].value = eeprom_read_float(4 * i);
	}
	
	for (uint8_t i = 0; i < coeff_list_SIZE; i++)
	{
		usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz] = coeffs[i].array[0];
		usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz + 1] = coeffs[i].array[1];
	}
}

bool Uint32Comparrer(uint32_t A1, uint32_t A2, uint32_t B1, uint32_t B2)
{
	if (A1 != B1 || A2 != B2)
	{
		return false;
	}
	
	return true;
}

void ModbusEEPROMLoader()
{
	uint8_t sond_flag = 0;

	
	for (uint8_t i = 0; i < coeff_list_SIZE; i++)
	{
		if (!Uint32Comparrer(usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz], usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz + 1], coeffs[i].array[0], coeffs[i].array[1]))
		{
			coeffs[i].array[0]=usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz];
			coeffs[i].array[1]=usRegHoldingBuf[2 * i + MB_OFFSET + MB_Kp_Oz + 1];
			eeprom_write_float(i * 4, coeffs[i].value);
			sond_flag = 1;
		}
	}	
	
	if (sond_flag)
	{
		Sound_On();
		_delay_ms(100);
		Sound_Off();		
	}
	
	ModbusInitValues(); 
}