/*
 * PC-SIMV_ASSIST_OFF.h
 *
 *  Created on: Feb 14, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_OFF_H_
#define INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_OFF_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef enum
{
	ONE_TIME_TRIGGER,
	COUNTINOUS_TRIGGER
}trigger_process;


typedef struct
{
	int  Blower_Wait_Time_Milli_Second;
	int  Trigger_Check_Wait_Time;
}pcsimv;




typedef struct
{
	uint16_t DAC_VALUE_5;
	uint16_t DAC_VALUE_6;
	uint16_t DAC_VALUE_7;
	uint16_t DAC_VALUE_8;
	uint16_t DAC_VALUE_9;
	uint16_t DAC_VALUE_10;
	uint16_t DAC_VALUE_11;
	uint16_t DAC_VALUE_12;
	uint16_t DAC_VALUE_13;
	uint16_t DAC_VALUE_14;
	uint16_t DAC_VALUE_15;
	uint16_t DAC_VALUE_16;
	uint16_t DAC_VALUE_17;
	uint16_t DAC_VALUE_18;
	uint16_t DAC_VALUE_19;
	uint16_t DAC_VALUE_20;
	uint16_t DAC_VALUE_21;
	uint16_t DAC_VALUE_22;
	uint16_t DAC_VALUE_23;
	uint16_t DAC_VALUE_24;
	uint16_t DAC_VALUE_25;
	uint16_t DAC_VALUE_26;
	uint16_t DAC_VALUE_27;
	uint16_t DAC_VALUE_28;
	uint16_t DAC_VALUE_29;
	uint16_t DAC_VALUE_30;
	uint16_t DAC_VALUE_31;
	uint16_t DAC_VALUE_32;
	uint16_t DAC_VALUE_33;
	uint16_t DAC_VALUE_34;
	uint16_t DAC_VALUE_35;
	uint16_t DAC_VALUE_36;
	uint16_t DAC_VALUE_37;
	uint16_t DAC_VALUE_38;
	uint16_t DAC_VALUE_39;
	uint16_t DAC_VALUE_40;
	uint16_t DAC_VALUE_41;
	uint16_t DAC_VALUE_42;
	uint16_t DAC_VALUE_43;
	uint16_t DAC_VALUE_44;
	uint16_t DAC_VALUE_45;
	uint16_t DAC_VALUE_46;
	uint16_t DAC_VALUE_47;
	uint16_t DAC_VALUE_48;
	uint16_t DAC_VALUE_49;
	uint16_t DAC_VALUE_50;
	uint16_t DAC_VALUE_51;
	uint16_t DAC_VALUE_52;
	uint16_t DAC_VALUE_53;
	uint16_t DAC_VALUE_54;
	uint16_t DAC_VALUE_55;
	uint16_t DAC_VALUE_56;
	uint16_t DAC_VALUE_57;
	uint16_t DAC_VALUE_58;
	uint16_t DAC_VALUE_59;
	uint16_t DAC_VALUE_60;
}OVER_ALL_DAC_SIGNAL;



pcsimv PCsimv;
OVER_ALL_DAC_SIGNAL DAC_SIGNALS;

xTaskHandle Pc_Simv_Assist_OFF_Handler;
void Pc_Simv_Assist_OFF_Task(void *argument);

void PC_SIMV_Assist_OFF_Inspiration_Time_Variables_Reset();
void PC_SIMV_Assist_OFF_Expiration_Time_Variables_Reset();



uint16_t TUNE_PEEP_BLOWER(uint16_t *,uint8_t);
trigger_process Trigger_Process;


#endif /* INC_AERO_INC_PC_SIMV_PC_SIMV_ASSIST_OFF_H_ */
