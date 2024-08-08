/*
 * Vc_Cmv.h
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */

#ifndef INC_VC_CMV_H_
#define INC_VC_CMV_H_

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

}VC_CMV_mode_parameter;



typedef enum
{
	Flow_fine,
	Low_Flow_Problem,
	High_Flow_Problem
}Flow_status;

typedef enum
{
	Not_Detect,
	Detect
}set_flow_value;


typedef enum
{
	BlOWER_ON,
	BlOWER_OFF
}Blower_stat;

Flow_status Flow_increment;

set_flow_value Set_Flow_value;
uint16_t LOW_COMPLIANCE_DAC;


VC_CMV_mode_parameter VC_CMV;

xTaskHandle Vc_Cmv_Handler;
void Vc_Cmv_Task (void *argument);


void VC_CMV_Inspiration_Time_Variables_Reset();
void VC_CMV_Expiration_Time_Variables_Reset();
void Volume_Not_Acheieved();
void Volume_Acheieved();
void Reset_Blower_Status();
uint16_t LOW_Compliance_DAC_value(uint8_t);


#endif /* INC_VC_CMV_H_ */
