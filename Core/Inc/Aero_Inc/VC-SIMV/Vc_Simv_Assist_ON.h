/*
 * Vc-Simv_Assist_ON.h
 *
 *  Created on: Feb 17, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_ON_H_
#define INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_ON_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct
{
	uint16_t VOLUME_Val;
	uint8_t  PEEP_Val;
	uint8_t  RESPIRATORY_RATE_Val;
	uint8_t  FIO2_Val;
	uint32_t INSPIRATION_TIME;
	uint32_t EXPIRATION_TIME;
	uint8_t  Inspiration_Time_Ratio;
	uint8_t  Expiration_Time_Ratio;
	uint8_t  FLOW_RATE_Val;

	uint8_t  TRIG_TYPE;
	float  TRIG_LMT;
	uint16_t  TRIG_TIME;
	uint16_t CALC_TRIG_VAL;
	uint16_t TOLERANCE_EWAVE;
	uint16_t TRIG_WINDOW;
	uint8_t  Assist_Control;

}VC_SIMV_mode_parameter;


VC_SIMV_mode_parameter VC_SIMV;

xTaskHandle Vc_Simv_Assist_ON_Handler;
void Vc_Simv_Assist_ON_Task(void *argument);
void VC_SIMV_Expiration_Time_Variables_Reset();
void VC_SIMV_Inspiration_Time_Variables_Reset();
void VC_SIMV_Assist_ON_Expiration_Time_Variables_Reset();
void VC_SIMV_Assist_ON_Inspiration_Time_Variables_Reset();

#endif /* INC_AERO_INC_VC_SIMV_VC_SIMV_ASSIST_ON_H_ */
