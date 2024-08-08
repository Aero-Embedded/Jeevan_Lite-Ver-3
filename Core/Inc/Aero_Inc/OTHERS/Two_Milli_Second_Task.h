/*
 * Two_Milli_Second_Task.h
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */

#ifndef INC_TWO_MILLI_SECOND_TASK_H_
#define INC_TWO_MILLI_SECOND_TASK_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



int Current_Pressure,Over_Pressure_Count;

xTaskHandle Time_Trace_Handler;
void Time_Trace_Task(void *argument);

void INSPIRATION_TIME_TRACE();
void EXPIRATION_TIME_TRACE();
void BREATH_COUNT();
void PC_SIMV_BLOWER_WAIT_TIME_TRACE();
void VC_SIMV_BLOWER_WAIT_TIME_TRACE();
void PSV_APNEA_COUNTER_TRACE();
void BIPAP_APNEA_COUNTER_TRACE();
void CPAP_APNEA_COUNTER_TRACE();
void APRV_APNEA_COUNTER_TRACE();

#endif /* INC_TWO_MILLI_SECOND_TASK_H_ */
