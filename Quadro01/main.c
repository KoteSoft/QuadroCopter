/*
* Quadro01.c
*
* Created: 01.06.2013 19:14:56
*  Author: Слава
*/



#include "main.h"

USHORT   usRegInputBuf[REG_INPUT_NREGS];
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

unsigned char ucRegCoilsBuf[REG_COILS_SIZE / 8];

volatile uint8_t flag=5;//7
volatile uint16_t sense[4];
volatile uint8_t i=0;

volatile uint8_t Precounter=0;



int main(void)
{
	/*Ждем пока все включится*/
	_delay_ms(100);
	
	/*Настраиваем порты ввода-вывода*/
	DDRB = 1<<PORTB0|1<<PORTB1|1<<PORTB2|1<<PORTB3|1<<PORTB4|1<<PORTB5|1<<PORTB6|1<<PORTB7;
    DDRC = 1<<PORTC0|1<<PORTC1|1<<PORTC2|0<<PORTC3|0<<PORTC4|0<<PORTC5|0<<PORTC6|0<<PORTC7;
    DDRD = 0<<PORTD0|0<<PORTD1|1<<PORTD2|0<<PORTD3|1<<PORTD4|0<<PORTD5|0<<PORTD6|0<<PORTD7;

    PORTB = 1;
	PORTD = 1 << PORTD2;
    
	/*Тяга двигателей на минимум*/
    for(uint8_t k = 0; k < CHANNELS_COUNT; ++k)
    {
        counter[k] = LOW;
    }
	
	/*Настраиваем I2C*/
	 TWSR = 0x00;
	 TWBR = ((F_CPU / I2C_SPEED) - 16) / 2;  
	 _delay_us(10);

	/*Включаем Таймер0*/
	TCCR0 = 1<<CS02 | 0<<CS01 | 0<<CS00;
	
	/*Включаем Таймер1*/
    OCR1A=HIGH; //TOP
    TCCR1A=0<<COM1A1|0<<COM1A0|1<<COM1B1|0<<COM1B0|0<<FOC1A|0<<FOC1B|1<<WGM11|1<<WGM10;
    TCCR1B=0<<ICNC1|0<<ICES1|1<<WGM13|1<<WGM12|0<<CS12|0<<CS11|1<<CS10;
    TIMSK= 1<<TOIE2 | 1<<OCIE1A|1<<OCIE1B|0<<TOIE1|1<<TOIE0|0<<OCIE0;
    OCR1B=LOW;
	
	/*Включаем АЦП*/
    ADC_Init();   
    
	/*Разрешаем работу прерываний*/
    sei();
	
	/*Настраиваем Modbus*/
	eMBErrorCode eStatus = eMBInit( MB_RTU, 0x01, 0, 57600, MB_PAR_NONE );
	eStatus = eMBEnable();
    
	/*Настраиваем сенсоры*/    
	SensorsInit();
	
	/*Загружаем в Holding Registers и в массив параметров значения из EEPROM*/
	ModbusInitValues();
		
    while(1)
    {
		/*Актуализируем значения Modbus-регистров в соответствии со значениями параметров*/
		ModbusLoader();
		/*Актуализируем значения параметров в соответствии со значениями Holding Registers*/
		ModbusSaver();
		
		/*Итерация Modbus*/
		eMBPoll();
		
		/*Ресурсоемкий расчет курса*/		
		Course_Calc();
    }
}

eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
    && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
            ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
            ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
    ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
            case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
            * protocol stack. */
            case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}



eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}
