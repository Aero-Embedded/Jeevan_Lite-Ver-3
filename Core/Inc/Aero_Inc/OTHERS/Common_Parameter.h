/*
 * Common_Parameter.h
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */

#ifndef INC_COMMON_PARAMETER_H_
#define INC_COMMON_PARAMETER_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

typedef struct
{
	uint8_t  _PIP_Val;
	uint8_t  _PEEP_Val;
	uint8_t  _RESPIRATORY_RATE_Val;
	uint8_t  _FIO2_Val;
	uint16_t _VOLUME_Val;
	uint8_t  _FLOW_RATE;
	uint16_t  TOLERANCE_EWAVE;
	uint16_t  TRIG_WINDOW;
	uint16_t  TRIG_LMT;
	uint16_t  TRIG_TIME;
}Common_Mode_parameter;


typedef struct
{
	float    PEEP_Temp;
	int16_t  Expiratory_Valve_Open_Time;
	uint8_t  Maximum_PEEP_Acheived;
	uint32_t PEEP_Cumulative_Val;
	uint32_t PEEP_Event_Count;
	uint8_t  PEEP_AVG_VAL;
	uint32_t PEEP_Cumulative_Val_Trigger;
	uint32_t PEEP_Event_Count_Trigger;
	int      CHECK_TRIGGER;

}PEEP_maintain_parameter;


typedef struct
{
	uint8_t  Maximum_PIP_Acheived;
	uint32_t PIP_Cumulative_Val;
	uint32_t PIP_Event_Count;
	uint8_t  PIP_AVG_VAL;

}PIP_average_parameter;

typedef struct
{
	int _INSPIRATION_TIME;
	int _EXPIRATION_TIME;
	int INSPIRATION_TIME_ACHEIVED;
	int EXPIRATION_TIME_ACHEIVED;
	int FLOW_ACHIEVED_TIMER;

}TIME_base_parameter;


typedef enum
{
	PEEP_NOT_ACHEIVED,
	PEEP_ACHEIVED
}Peep_status;

typedef enum
{
	OPENED,
	CLOSED
}Expiratory_Valve_Lock_delay;

typedef enum
{
	INSPIRATION_CYCLE = 0,
	EXPIRATION_CYCLE  = 1,
	PATIENT_TRIGGER_INSPIRATION   = 2,
	PATIENT_TRIGGER_EXPIRATION    = 3,
	NO_CYCLE
}Breathe_state;

typedef enum
{
	RESUME,
	PAUSE
}Device_control;


Peep_status Peep_Status;
Expiratory_Valve_Lock_delay Expiratory_Valve_Lock_Delay;
Common_Mode_parameter Common_Mode_Parameter;
PIP_average_parameter PIP_Average_Parameter;
TIME_base_parameter TIME_Base_parameter;

PEEP_maintain_parameter PEEP_Maintain_Parameter;
Breathe_state BREATH_STATE;


void Find_Max_PIP();
void Find_Maximum_PEEP();
void PIP_AVERAGE();
void PEEP_AVERAGE();
void INITIAL_DAC_VALUE();
uint16_t DAC_VAL(uint8_t);
void Change_DAC_Values(uint8_t,uint16_t);

#endif /* INC_COMMON_PARAMETER_H_ */
