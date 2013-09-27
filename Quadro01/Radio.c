/*
 * Radio.c
 *
 * Created: 09.08.2013 2:20:54
 *  Author: Слава
 */ 
#include "Radio.h"

uint8_t reciving;
uint8_t rx_index;
uint8_t rx_buffer[BUFFER_SIZE];
uint8_t rx_data[BUFFER_SIZE - 2];

uint8_t RX_Check();

void Byte_Recive(uint8_t Byte)
{
	if (!reciving)
	{
		if (Byte == coeffs[My_ID].array[0])
		{
			reciving = 1;
		}
	}
	else
	{
		rx_buffer[rx_index] = Byte;
		rx_index++;
		
		if(rx_index >= BUFFER_SIZE)
		{
			rx_index = 0;
			RX_Check();
		}
	}
}

uint8_t RX_Check()
{
	memcpy(rx_data, rx_buffer, BUFFER_SIZE - 2);
	uint16_t rx_CRC = 123;//usMBCRC16(rx_data, BUFFER_SIZE - 2);
	if (rx_CRC == (uint16_t)(rx_buffer[16]<<8 | rx_buffer[15]))
	{
		usRegHoldingBuf[MB_OFFSET+MB_ALL] = rx_data[2]<<8 | rx_data[1];
		Ox.array[0] = rx_data[4]<<8 | rx_data[3];
		Ox.array[1] = rx_data[6]<<8 | rx_data[5];
		Oy.array[0] = rx_data[8]<<8 | rx_data[7];
		Oy.array[1] = rx_data[10]<<8 | rx_data[9];
		Oz.array[0] = rx_data[12]<<8 | rx_data[11];
		Oz.array[1] = rx_data[14]<<8 | rx_data[13];

		return 0;
	}

	return 1;
}