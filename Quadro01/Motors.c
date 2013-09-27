/*
 * Motors.c
 *
 * Created: 12.06.2013 18:08:22
 *  Author: Слава
 */ 

#include "Motors.h"

uint8_t motor_index = 0;
volatile uint16_t counter[CHANNELS_COUNT];

void SetMotors(double_t* motors)
{
    for (uint8_t i = 0; i < MOTORS_COUNT; ++i)
    {
        uint16_t sum = (uint16_t)motors[i];

        if (sum > LOW && sum < HIGH)
        {
            counter[i] = sum;
        }
        else if (sum < LOW)
        {
            counter[i] = LOW;
        }
        else if (sum > HIGH)
        {
            counter[i] = HIGH;
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    
    if (motor_index <= 7)
    {
        OCR1B = counter[motor_index];
    }
    else
    {
        OCR1B = 0;
    }
    
}

ISR(TIMER1_COMPB_vect, ISR_NOBLOCK)
{
    if (motor_index <= 7)
    {
        PORTB = motor_index | PORTB & 0b11111000;
    }
    
    if (motor_index >= 9)
    {
        motor_index = 0;
    }
    else
    {
        motor_index++;
    }
}