/*
 * i2c.c
 *
 * Created: 25.06.2013 21:20:37
 *  Author: Слава
 */ 

#include "i2c.h"

uint8_t twi(uint8_t action)
{
	switch(action)
	{
		case TWI_START:
		case TWI_RESTART:
		TWCR = _BV(TWSTA) | _BV(TWEN) | _BV(TWINT);// Если нужно прерывание | _BV(TWIE);
		break;
		case TWI_STOP:
		TWCR = _BV(TWSTO) | _BV(TWEN) | _BV(TWINT);// | _BV(TWIE);
		break;
		case TWI_TRANSMIT:
		TWCR = _BV(TWEN) | _BV(TWINT);// | _BV(TWIE);
		break;
		case TWI_RECEIVE_ACK:
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);//| _BV(TWIE);
		break;
		case TWI_RECEIVE_NACK:
		TWCR = _BV(TWEN) | _BV(TWINT);// | _BV(TWIE);
		break;
	}
	if(action != TWI_STOP)
		while (!(TWCR & _BV(TWINT)));
	return (TWSR & 0xF8);
}