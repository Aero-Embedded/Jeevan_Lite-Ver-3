/*
 * Backup_Pc_cmv.h
 *
 *  Created on: Feb 21, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_BACKUP_PC_CMV_BACKUP_PC_CMV_H_
#define INC_AERO_INC_BACKUP_PC_CMV_BACKUP_PC_CMV_H_



#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



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
}BACKUP_PC_CMV_mode_parameter;


xTaskHandle BACKUP_PC_Handler;
BACKUP_PC_CMV_mode_parameter BACKUP_PC_CMV;


void BACKUP_PC_CMV_Task(void *argument);
void BACKUP_PC_CMV_Inspiration_Time_Variables_Reset();
void BACKUP_PC_CMV_Expiration_Time_Variables_Reset();
void START_PSV();
void START_BIPAP();
void START_CPAP();

#endif /* INC_AERO_INC_BACKUP_PC_CMV_BACKUP_PC_CMV_H_ */
