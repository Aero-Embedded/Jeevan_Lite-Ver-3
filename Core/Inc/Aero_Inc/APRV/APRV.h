/*
 * APRV.h
 *
 *  Created on: Feb 27, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_APRV_APRV_H_
#define INC_AERO_INC_APRV_APRV_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


typedef struct
{
	uint8_t P_HIGH;
	uint8_t P_LOW;
	uint8_t T_HIGH;
	uint8_t T_LOW;
	uint32_t T_HIGH_VAL;
	uint32_t T_LOW_VAL;
	uint32_t T_LOW_VAL_COUNTER;
	uint32_t T_HIGH_VAL_COUNTER;
	uint8_t  FIO2_VAL;
}aprv_mode_Parameter;



typedef enum
{
	APRV_Idle,
	APRV_INSPIRATION,
	APRV_EXPIRATION

}aprv_breathe;



typedef enum
{
	APRV_IDLE,
	APRV_INS,
	APRV_EXP
}aprv_current_breathe;


typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t LAST_BLOWER_DAC_VAL;
	uint16_t BLOWER_ENDING_DAC_SIGNAL;
	float    Blower_Signal_Voltage;
	float    BLOWER_VOLTAGE_SIGNAL;

}aprv_mode_blower_control;


typedef struct
{
	uint32_t PIP_Cumulative_Val;
	uint32_t PIP_Event_Count;
	uint8_t  PIP_AVG_VAL;
}Aprv_PIP_average;



typedef struct
{
	uint32_t PEEP_Cumulative_Val;
	uint32_t PEEP_Event_Count;
	uint8_t  PEEP_AVG_VAL;
}Aprv_PEEP_average;



xTaskHandle APRV_Handler;
xTaskHandle	APRV_ONE_TIME_Handler;


aprv_breathe APRV_Next_Breathe;
aprv_current_breathe APRV_current_breathe;
aprv_mode_blower_control APRV_mode_blower_control;
Aprv_PIP_average APRV_PIP_average;
Aprv_PEEP_average APRV_PEEP_average;

void APRV_Task(void *argument);
void APRV_ONE_TIME_Task(void *argument);



aprv_mode_Parameter APRV;

#endif /* INC_AERO_INC_APRV_APRV_H_ */
