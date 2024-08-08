/*
 * Vc-Simv_Assist_OFF.h
 *
 *  Created on: Feb 17, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_OFF_H_
#define INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_OFF_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct
{
	int  Blower_Wait_Time_Milli_Second;
	int  Trigger_Check_Wait_Time;
}vcsimv;

vcsimv VCsimv;

xTaskHandle Vc_Simv_Assist_OFF_Handler;
void Vc_Simv_Assist_OFF_Task(void *argument);
void VC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Expiration_Time_Variables_Reset();
void VC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Inspiration_Time_Variables_Reset();
void VC_SIMV_Assist_OFF_Expiration_Time_Variables_Reset();
void VC_SIMV_Assist_OFF_Inspiration_Time_Variables_Reset();

#endif /* INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_OFF_H_ */
