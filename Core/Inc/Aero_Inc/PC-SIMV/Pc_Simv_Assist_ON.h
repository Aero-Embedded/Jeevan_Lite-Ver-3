/*
 * PC-SIMV_ASSIST_ON.h
 *
 *  Created on: Feb 14, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_ON_H_
#define INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_ON_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


#define E_TIME_TOLERANCE 30u

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
	uint8_t  TRIG_TYPE;
	float    TRIG_LMT;
	uint16_t  TRIG_TIME;
	uint16_t CALC_TRIG_VAL;
	uint16_t TOLERANCE_EWAVE;
	uint16_t TRIG_WINDOW;
	uint8_t  Assist_Control;

}PC_SIMV_mode_parameter;


typedef enum
{
	ASSIST_ON,
	ASSIST_OFF
}assist;

typedef enum
{
	Pressure_Trigger,
	Flow_Trigger
}trigger_type;


typedef enum
{
	NO_PATIENT_TRIGGER,
	PATIENT_TRIGGER_HAPPEN
}Patient_trigger;

xTaskHandle Pc_Simv_Assist_ON_Handler;
void Pc_Simv_Assist_ON_Task(void *argument);

void PC_SIMV_Inspiration_Time_Variables_Reset();
void PC_SIMV_Expiration_Time_Variables_Reset();
void PC_SIMV_Assist_ON_Inspiration_Time_Variables_Reset();
void PC_SIMV_Assist_ON_Expiration_Time_Variables_Reset();




PC_SIMV_mode_parameter PC_SIMV;
assist Assist_Control;
trigger_type Trigger_Type;
Patient_trigger Patient_Trigger;
#endif /* INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_ON_H_ */
