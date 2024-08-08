/*
 * Cpap.h
 *
 *  Created on: Feb 27, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_CPAP_CPAP_H_
#define INC_AERO_INC_CPAP_CPAP_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct
{
	uint8_t   CPAP_Val;
	uint16_t  TRIG_TYPE;
    float  TRIG_LMT;
	uint16_t  TRIG_TIME;
	uint16_t  CALC_TRIG_VAL;
	uint16_t  TOLERANCE_EWAVE;
	uint16_t  TRIG_WINDOW;
	uint16_t  APNEA_TIME;
	int       APNEA_COUNTER;
}cpap_mode_Parameter;




typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t CPAP_BLOWER_DAC_VAL;
	uint16_t LAST_CPAP_BLOWER_DAC_VAL;
	float    Blower_Signal_Voltage;
	float    BLOWER_VOLTAGE_SIGNAL;

}Cpap_mode_blower_control;

typedef struct
{
	int CPAP_Settle_Time;
	int Blower_Wait_Time_Milli_Second;
	int Trigger_Check_Wait_Time;
	int _INSPIRATION_TIME;
}Cpap_Flags;


typedef struct
{
	uint32_t CPAP_Cumulative_Val;
	uint32_t CPAP_Event_Count;
	uint8_t  CPAP_AVG_VAL;
}cpap_average;



xTaskHandle CPAP_Handler;
void CPAP_Task(void *argument);

cpap_mode_Parameter CPAP;
Cpap_Flags CPAP_Control;
Cpap_mode_blower_control CPAP_Mode_Blower_control;
cpap_average CPAP_Average;

#endif /* INC_AERO_INC_CPAP_CPAP_H_ */
