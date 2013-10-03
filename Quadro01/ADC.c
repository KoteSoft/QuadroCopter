/*
 * ADC.c
 *
 * Created: 02.10.2013 21:30:00
 *  Author: Слава
 */ 

#include "ADC.h"

ISR(ADC_vect)
{
	U_Batt.value = (ADC * 35.45) / 1024.0 + D4_offset;
	//U_Batt.array[0] = ADCH<<8 | ADCL;
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
}

void ADC_Init()
{
	ADCSRA=1<<ADEN|1<<ADSC|0<<ADATE|0<<ADIF|1<<ADIE|1<<ADPS2|1<<ADPS1|1<<ADPS0;
	SFIOR = 0<<ADTS2 | 0<<ADTS1 | 0<<ADTS0;
	ADMUX=0;	
}