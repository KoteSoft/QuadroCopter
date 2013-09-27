/*
 * PIDs.c
 *
 * Created: 12.06.2013 17:47:52
 *  Author: Слава
 */ 

#include "PIDs.h"

double_t motors_sum[MOTORS_COUNT];

void PID_Oz()
{
	double_t D = Heading_displacement_calc(Oz.value, t_Oz.value);
	
	motors_sum[FRONT_LEFT] += coeffs[Kp_Oz].value * D * -1.0;
	motors_sum[FRONT_RIGHT] += coeffs[Kp_Oz].value * D;
	motors_sum[REAR_LEFT] += coeffs[Kp_Oz].value * D;
	motors_sum[REAR_RIGHT] += coeffs[Kp_Oz].value * D * -1.0;	
	
	motors_sum[FRONT_LEFT] += coeffs[Kd_Oz].value * w_z.value * -1.0;
	motors_sum[FRONT_RIGHT] += coeffs[Kd_Oz].value * w_z.value;
	motors_sum[REAR_LEFT] += coeffs[Kd_Oz].value * w_z.value;
	motors_sum[REAR_RIGHT] += coeffs[Kd_Oz].value * w_z.value * -1.0;
}

void PID_Ox()
{
	motors_sum[FRONT_LEFT] += coeffs[Kp_Ox].value * (Ox.value - t_Ox.value) * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kp_Ox].value * (Ox.value - t_Ox.value);
	motors_sum[REAR_LEFT] += coeffs[Kp_Ox].value * (Ox.value - t_Ox.value) * (-1.0);
	motors_sum[REAR_RIGHT] += coeffs[Kp_Ox].value * (Ox.value - t_Ox.value);
	
	motors_sum[FRONT_LEFT] += coeffs[Ki_Ox].value * int_Ox.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Ki_Ox].value * int_Ox.value;
	motors_sum[REAR_LEFT] += coeffs[Ki_Ox].value * int_Ox.value * (-1.0);
	motors_sum[REAR_RIGHT] += coeffs[Ki_Ox].value * int_Ox.value;
	
	motors_sum[FRONT_LEFT] += coeffs[Kd_Ox].value * w_x.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kd_Ox].value * w_x.value;
	motors_sum[REAR_LEFT] += coeffs[Kd_Ox].value * w_x.value * (-1.0);
	motors_sum[REAR_RIGHT] += coeffs[Kd_Ox].value * w_x.value;
}

void PID_Oy()
{
	/*
	motors_sum[FRONT_LEFT] += coeffs[Kp_Oy].value * (Oy.value - t_Oy.value) * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kp_Oy].value * (Oy.value - t_Oy.value) * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Kp_Oy].value * (Oy.value - t_Oy.value);
	motors_sum[REAR_RIGHT] += coeffs[Kp_Oy].value * (Oy.value - t_Oy.value);

	motors_sum[FRONT_LEFT] += coeffs[Ki_Oy].value * int_Oy.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Ki_Oy].value * int_Oy.value * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Ki_Oy].value * int_Oy.value;
	motors_sum[REAR_RIGHT] += coeffs[Ki_Oy].value * int_Oy.value;
	
	motors_sum[FRONT_LEFT] += coeffs[Kd_Oy].value * w_y.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kd_Oy].value * w_y.value * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Kd_Oy].value * w_y.value;
	motors_sum[REAR_RIGHT] += coeffs[Kd_Oy].value * w_y.value;
	*/
	motors_sum[FRONT_LEFT] += coeffs[Kp_Ox].value * (Oy.value - t_Oy.value) * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kp_Ox].value * (Oy.value - t_Oy.value) * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Kp_Ox].value * (Oy.value - t_Oy.value);
	motors_sum[REAR_RIGHT] += coeffs[Kp_Ox].value * (Oy.value - t_Oy.value);

	motors_sum[FRONT_LEFT] += coeffs[Ki_Ox].value * int_Oy.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Ki_Ox].value * int_Oy.value * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Ki_Ox].value * int_Oy.value;
	motors_sum[REAR_RIGHT] += coeffs[Ki_Ox].value * int_Oy.value;
	
	motors_sum[FRONT_LEFT] += coeffs[Kd_Ox].value * w_y.value * (-1.0);
	motors_sum[FRONT_RIGHT] += coeffs[Kd_Ox].value * w_y.value * (-1.0);
	motors_sum[REAR_LEFT] += coeffs[Kd_Ox].value * w_y.value;
	motors_sum[REAR_RIGHT] += coeffs[Kd_Ox].value * w_y.value;
}

void RunPIDs()
{
	for (uint8_t i = 0; i< MOTORS_COUNT; i++)
	{
		motors_sum[i]=usRegHoldingBuf[MB_OFFSET+MB_ALL];
	}
	
	PID_Oz();
	PID_Ox();
	PID_Oy();
		
	SetMotors(motors_sum);
}
