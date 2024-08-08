/*
 * Pc_cmv.h
 *
 *  Created on: Jan 20, 2023
 *      Author: asus
 */

#ifndef INC_PC_CMV_H_
#define INC_PC_CMV_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


#define One_Minite_In_MS 60000


typedef struct
{
	uint8_t  PIP_Val;
	uint8_t  PEEP_Val;
	uint8_t  RESPIRATORY_RATE_Val;
	uint8_t  FIO2_Val;
	float    RISE_TIME_MS_Val;
	uint32_t INSPIRATION_TIME;
	uint32_t EXPIRATION_TIME;
	uint8_t  Inspiration_Time_Ratio;
	uint8_t  Expiration_Time_Ratio;
	uint8_t  Rise_Time;

}PC_CMV_mode_parameter;



typedef enum
{
	Smooth_Ramp_Time_Val_Pending,
	Smooth_Ramp_Time_Val_Finished
}smooth_ramp_time;


typedef enum
{
	Leave_Count_The_Breath,
	Count_The_Breath
}breath;


breath Breath_Count;

smooth_ramp_time Smooth_Ramp_Time;

PC_CMV_mode_parameter PC_CMV;


xTaskHandle Pc_Cmv_Handler;
void Pc_Cmv_Task (void *argument);

void PC_CMV_Inspiration_Time_Variables_Reset();
void PC_CMV_Expiration_Time_Variables_Reset();
void PIP_Not_Acheieved();
void PIP_Acheieved();


#endif /* INC_PC_CMV_H_ */
