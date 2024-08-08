/*
 * PSV.h
 *
 *  Created on: Feb 18, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_PSV_PSV_H_
#define INC_AERO_INC_PSV_PSV_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct
{
	uint16_t  PRESSURE_SUPPORT_Val;
	uint8_t   PEEP_Val;
	uint16_t  TRIG_TYPE;
    float     TRIG_LMT;
	uint16_t  TRIG_TIME;
	uint16_t  CALC_TRIG_VAL;
	uint16_t  TOLERANCE_EWAVE;
	uint16_t  TRIG_WINDOW;
	uint16_t  APNEA_TIME;
	int       APNEA_COUNTER;
}psv_mode_Parameter;


typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t LAST_BLOWER_DAC_VAL;
	uint16_t BLOWER_ENDING_DAC_SIGNAL;
	uint16_t PEEP_BLOWER_DAC_VAL;
	uint16_t LAST_PEEP_BLOWER_DAC_VAL;
	float    Blower_Signal_Voltage;
	float    BLOWER_VOLTAGE_SIGNAL;

}Psv_mode_blower_control;

typedef struct
{
	int PSV_PEEP_Settle_Time;
	int Blower_Wait_Time_Milli_Second;
	int Trigger_Check_Wait_Time;
	int _INSPIRATION_TIME;
}Psv_Flags;


typedef struct
{
	uint32_t PEEP_Cumulative_Val;
	uint32_t PEEP_Event_Count;
	uint8_t  PEEP_AVG_VAL;
}psv_peep_average;

typedef struct
{
	uint8_t  Maximum_PRESSURE_SUPPORT_Acheived;
	uint32_t PRESSURE_SUPPORT_Cumulative_Val;
	uint32_t PRESSURE_SUPPORT_Event_Count;
	uint8_t  PRESSURE_SUPPORT_AVG_VAL;

}psv_pip_average;



typedef enum
{
	NORMAL_PRESSURE,
	OVER_PRESSURE
}pressure_status;

xTaskHandle PSV_Handler,PSV_PID_Handler;
psv_mode_Parameter PSV;
Psv_Flags PSV_Control;
Psv_mode_blower_control PSV_Mode_Blower_control;
psv_peep_average PSV_Peep_Average;
psv_pip_average PSV_Pip_average;
pressure_status Pressure_Status;

void PSV_Task(void *argument);
void PSV_PID_Task(void *argument);



#endif /* INC_AERO_INC_PSV_PSV_H_ */
