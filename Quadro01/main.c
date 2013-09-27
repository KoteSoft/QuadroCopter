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

ISR(ADC_vect)
{
	U_Batt.value = (ADC * 35.45) / 1024.0 + D4_offset;
	//U_Batt.array[0] = ADCH<<8 | ADCL;
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
}

int main(void)
{
	_delay_ms(100);
	
	DDRB = 1<<PORTB0|1<<PORTB1|1<<PORTB2|1<<PORTB3|1<<PORTB4|1<<PORTB5|1<<PORTB6|1<<PORTB7;
    DDRC = 1<<PORTC0|1<<PORTC1|1<<PORTC2|0<<PORTC3|0<<PORTC4|0<<PORTC5|0<<PORTC6|0<<PORTC7;
    DDRD = 0<<PORTD0|0<<PORTD1|1<<PORTD2|0<<PORTD3|1<<PORTD4|0<<PORTD5|0<<PORTD6|0<<PORTD7;

    PORTB = 1;
	PORTD = 1 << PORTD2;
    
    for(uint8_t k = 0; k < CHANNELS_COUNT; ++k)
    {
        counter[k] = LOW;
    }
	
	 TWSR = 0x00;
	 //TWBR = 10;
	 TWBR = ((F_CPU / I2C_SPEED) - 16) / 2;       // change the I2C clock rate
	 _delay_us(10);

	TCCR0 = 1<<CS02 | 0<<CS01 | 0<<CS00;
	//OCR0 = 57;

    OCR1A=HIGH; //TOP
    TCCR1A=0<<COM1A1|0<<COM1A0|1<<COM1B1|0<<COM1B0|0<<FOC1A|0<<FOC1B|1<<WGM11|1<<WGM10;
    TCCR1B=0<<ICNC1|0<<ICES1|1<<WGM13|1<<WGM12|0<<CS12|0<<CS11|1<<CS10;
    TIMSK= 1<<TOIE2 | 1<<OCIE1A|1<<OCIE1B|0<<TOIE1|1<<TOIE0|0<<OCIE0;
    
    //TCCR2 = 1<<CS22 | 1<<CS21 | 1<<CS20;
    
    
    ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
    SFIOR = 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0;
	ADMUX=0;
    
    OCR1B=LOW;
    sei();

	eMBErrorCode eStatus = eMBInit( MB_RTU, 0x01, 0, 57600, MB_PAR_NONE );
	//eMBErrorCode eStatus = eMBInit( MB_RTU, 0x01, 0, 56000, MB_PAR_ODD );
	eStatus = eMBEnable();
        
	SensorsInit();
	ModbusInitValues();
		
    while(1)
    {
		ModbusLoader();
		ModbusSaver();
		eMBPoll();
				
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
