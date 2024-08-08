/*
 * Mode_Initial_Flag_Set.h
 *
 *  Created on: Jan 21, 2023
 *      Author: asus
 */

#ifndef INC_MODE_INITIAL_FLAG_SET_H_
#define INC_MODE_INITIAL_FLAG_SET_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

typedef enum
{
	CLOSE,
	OPEN
}Mode_initial_flag_set;


typedef enum
{
	NO_CHANGES,
	CHANGE_HAPPEN
}set_value_Change;


typedef struct
{
	uint8_t Last_PEEP_Val_PC_CMV_BACKUP;
	uint8_t Last_PIP_Val_PC_CMV_BACKUP;
	uint8_t Last_Rise_Time_PC_CMV_BACKUP;
	uint8_t Last_O2_Val_PC_CMV_BACKUP;
	uint8_t Last_RR_Val_PC_CMV_BACKUP;
	uint8_t Last_I_Val_PC_CMV_BACKUP;
	uint8_t Last_E_Val_PC_CMV_BACKUP;
	float   Last_Ramp_Time_PC_CMV_BACKUP;
	uint16_t Last_Volume_Val_VC_CMV_BACKUP;
	uint8_t Last_PEEP_Val_VC_CMV_BACKUP;
	uint8_t Last_FLOW_Val_VC_CMV_BACKUP;
	uint8_t Last_O2_Val_VC_CMV_BACKUP;
	uint8_t Last_RR_Val_VC_CMV_BACKUP;
	uint8_t Last_I_Val_VC_CMV_BACKUP;
	uint8_t Last_E_Val_VC_CMV_BACKUP;
	uint16_t LAST_BLOWER_ENDING_DAC_SIGNAL;
	uint16_t LAST_PIP_Control_Task_Delay;
	uint16_t LAST_Increase_Each_DAC_SET_CONST;
	uint8_t LAST_Need_PID_Task_Delay;
	uint8_t LAST_Tune_PID_Task_Delay;
	uint8_t LAST_Low_PIP_Count;
	uint8_t LAST_Early_Acheived_Ramp_Time_Percentage;
}old_flags;

set_value_Change Backup_PC_CMV_SET_VALUE,Backup_VC_CMV_SET_VALUE;

Mode_initial_flag_set Mode_Initial_Flag_Set;
Mode_initial_flag_set Back_Up_Mode_Initial_Flag_Set;
old_flags OLD;


xTaskHandle Mode_initial_Flag_Set_Handler;


void Mode_Initial_Flag_Set_Task (void *argument);
void PC_CMV_Initial_Flags_SET();
void VC_CMV_Initial_Flags_SET();
void PC_SIMV_Initial_Flags_SET();
void VC_SIMV_Initial_Flags_SET();
void PSV_Initial_Flags_SET();
void CPAP_Initial_Flags_SET();
void BIBAP_Initial_Flags_SET();
void APRV_Initial_Flags_SET();
void Backup_PC_CMV_Initial_Flags_SET();
void Backup_VC_CMV_Initial_Flags_SET();


void Reset_PC_CMV_Old_Flags_Reset();
void Reset_VC_CMV_Old_Flags_Reset();
void Reset_PC_SIMV_Old_Flags_Reset();
void Reset_VC_SIMV_Old_Flags_Reset();
void Reset_PC_CMV_BACKUP_Old_Flags_Reset();
void Reset_VC_CMV_BACKUP_Old_Flags_Reset();

void Reset_Patient_Circuit_Diconnceted_Alert_Flags();


#endif /* INC_MODE_INITIAL_FLAG_SET_H_ */
